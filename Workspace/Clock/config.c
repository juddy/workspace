#include "wsclock.h"

/* load the variables from a predefined theme ****************************************/

void config ()
{
#include "default_theme/config"

  cal_x = (cal_x1+cal_x2)/2;
}

/*************************************************************************************/

static char *help_message[] = {
"where options include:",
"    -help                   this help message",
"    -12                     12 hour format",
"    -24                     24 hour format",
"    -dock                   enable Windowmaker docking",
"    -exe <program>          program to start on click",
"    -theme <theme-dir>      directory containing a set of xpms",
"    -position [+|-]x[+|-]y  position of wsclock",
"    -shape                  without groundplate",
"    -noblink                don't blink",
NULL
};

static char *themes_directories[] = {
  "/usr/local/share/wsclock/",
  "/usr/share/wsclock/",
  "./themes/",
  "",
  NULL
};

void usage(char *ProgName)
{
  char **cpp;
  DIR *dfd;
  struct dirent *dp;

  fprintf(stderr,"usage:  %s [-options ...] \n", ProgName);
  for (cpp = help_message; *cpp; cpp++) {
    fprintf(stderr, "%s\n", *cpp);
  }
  fprintf(stderr,"\n");

  /* list themes in all known themes directories */
  fprintf(stderr, "----------------------------------------------------------------\n");
  fprintf(stderr, "List of installed themes directories\n\n");

  for (cpp= themes_directories; *cpp; cpp++)
    {
      fprintf(stderr, "dir : %s\n", *cpp);

      if((dfd = opendir(*cpp)) == NULL)
	  printf(" not found\n\n"); 
      else
	while((dp = readdir(dfd)) != NULL)
	  if ( dp->d_name[0]!='.' )
	    printf("%s\n", dp->d_name);

      printf("\n\n");

      closedir(dfd);
    }

  exit(1);
}

int findTheme(char*input, char *ret)
{
  char **dir;
  DIR *d;
  int n;

  n = MAX_PATH_LEN - strlen(input);

  for(dir = themes_directories; *dir; dir++)
    {

      if( strlen(*dir) < n )
	{
	  strncpy(ret, *dir, MAX_PATH_LEN);
	  strncat(ret, input, MAX_PATH_LEN);      

	  if( (d = opendir(ret)) != NULL )
	  {
	    closedir(d);
	    return TRUE;
	  }
	}
    }
  return FALSE;
}

void loadTheme(char *filename)
{
  FILE *f;
  char token[64];
  int type;
  f = fopen(filename, "r");

  if(!f)
    {
      fprintf(stderr, "%s not found\n", filename);
      fprintf(stderr,"There is a problem with this theme.\n");
      exit(-1);
    }

  /* make sure nothing is accidentally visible */
  led_visible = 0;
  cal_visible = 0;
  week_visible = 0;
  month_visible = 0;
  analog_visible = 0;
  hour_visible = 0;
  min_visible = 0;
  sec_visible = 0;

  read_init(f);

  /* parse the config file for int's */
  while(read_type(&type))
    {
      symbol *s;

      if(!read_token(token, 64)) 
	printf("read_token failed\n");

      s = config_symbols;
      while((s->name) && (strcmp(s->name, token)!=0))
        s++;
     
      if(!(s->name)) {
	fprintf(stderr, "There is no such thing as a variable called %s\n", token);
	exit(-1);
      } 

      if(!read_assign(f))
	printf("read_assign failed\n");
      
      if(!read_int(s->addr))
	printf("read_int failed\n");
      
      if(!read_semicolon(f))
	printf("read_semicolon failed\n");
    }

  cal_x = (cal_x1 + cal_x2)/2;
  analog_visible = hour_visible || min_visible || sec_visible;

  fclose(f);

}

void parseArgs(int argc, char **argv)
{
  int i;
  char themesdir[MAX_PATH_LEN];
  char positions[MAX_PATH_LEN];
  char *ProgName = argv[0];

  itblinks=1;

  for(i=1;i<argc;i++) {
    char *arg= argv[i];

    if (arg[0] == '-') {
      switch(arg[1]) {
      case 'h':
	usage(ProgName);
	exit(0);

      case 'n':
	itblinks = 0;
	continue;
      case '1':
        showampm=1;
        continue;
      case '2':
        showampm=0;
        continue;
      case 't':
        if(++i >=argc) usage(ProgName);

        if(! findTheme(argv[i], themesdir) )
          {
            fprintf(stderr, "Could not find theme %s\n\n", argv[i]);
            exit(-1);
          }

        strcpy(positions, themesdir);
        strcat(positions, "/config");

        loadTheme(positions);

        strcpy(clock_xpm_fn, themesdir);
        strcat(clock_xpm_fn, "/clock.xpm");

        strcpy(month_xpm_fn, themesdir);
        strcat(month_xpm_fn, "/month.xpm");
        
        strcpy(weekday_xpm_fn, themesdir);
        strcat(weekday_xpm_fn, "/weekday.xpm");

        strcpy(date_xpm_fn, themesdir);
        strcat(date_xpm_fn, "/date.xpm");

        strcpy(led_xpm_fn, themesdir);
        strcat(led_xpm_fn, "/led.xpm");

        strcpy(hour_xpm_fn, themesdir);
        strcat(hour_xpm_fn, "/hour.xpm");

        strcpy(min_xpm_fn, themesdir);
        strcat(min_xpm_fn, "/minute.xpm");

        strcpy(sec_xpm_fn, themesdir);
        strcat(sec_xpm_fn, "/second.xpm");

        continue;
      case 'd':
	itdocks=1;
	continue;
      case 'e':
        if(++i >=argc) usage(ProgName);
        strcpy(exec_str, argv[i]);
        strcat(exec_str, " &");
        continue;

      }
    }
  }
}
