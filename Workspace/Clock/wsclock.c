#include "wsclock.h"
#include "version.h"

#include "default_theme/clock.xpm"
#include "default_theme/led.xpm"
#include "default_theme/month.xpm"
#include "default_theme/date.xpm"
#include "default_theme/weekday.xpm"

int ONLYSHAPE=0; /* default value is noshape */
int ICONIFIED=0; /* default is not iconified */
int YEAR=0;      /* default is to show time, not year */
int itdocks=0;      /* default is not Dock mode */
/* X11 Variablen *************************************************************/
Display *dpy;	  /* welches DISPLAY */
Window Root;      /* Hintergrund-Drawable */
int screen;
int x_fd;
int d_depth;
XSizeHints mysizehints;
XWMHints mywmhints;
Pixel back_pix, fore_pix;
GC NormalGC;
Window iconwin, win;       /* My home is my window */
char *Geometry;
char *LedColor = "LightSeaGreen";
char exec_str[] = "echo no program has been specified >/dev/console";
char *ERR_colorcells = "not enough free color cells or xpm not found\n";
char *ampers = " &";
char month_xpm_fn[MAX_PATH_LEN]="";
char clock_xpm_fn[MAX_PATH_LEN]="";
char weekday_xpm_fn[MAX_PATH_LEN]="";
char date_xpm_fn[MAX_PATH_LEN]="";
char led_xpm_fn[MAX_PATH_LEN]="";
char hour_xpm_fn[MAX_PATH_LEN]="";
char min_xpm_fn[MAX_PATH_LEN]="";
char sec_xpm_fn[MAX_PATH_LEN]="";
char positions[MAX_PATH_LEN]="";
/* XPM Variables *************************************************************/
typedef struct _XpmIcon {
    Pixmap pixmap;
    Pixmap mask;
    XpmAttributes attributes;
}        XpmIcon;

XpmIcon wsclock, led, month, date, weekday;
XpmIcon visible;
time_t actualtime;
long actualmin;

/* local functions *********************************************************/
#define MW_EVENTS   (ExposureMask | ButtonPressMask | StructureNotifyMask)

void GetXPM(void);
Pixel GetColor(char *name);
void RedrawWindow( XpmIcon *v);
void InsertTime();


static Atom wm_delete_window;

int main(int argc,char *argv[])
{
  int i;
  int ret;
  char themesdir[MAX_PATH_LEN] = "";
  unsigned int borderwidth ;
  char *display_name = NULL; 
  char *wname = "wsclock";
  XGCValues gcv;
  unsigned long gcm;
  XEvent Event;
  XTextProperty name;
  XClassHint classHint;
  int hadArguments = 0;
  Geometry = "";
  
  /* Parse command line options */
  config();
  parseArgs(argc, argv);

  if(analog_visible || hour_visible || min_visible || sec_visible) {
    fprintf(stderr, "%s does not support analog clocks yet.\n", version_id);
    fprintf(stderr, "You want the wsclock-gtk versions\n");
  }

  /* Open the display */
  if (!(dpy = XOpenDisplay(display_name)))  
    { 
      fprintf(stderr,"wsclock: can't open display %s\n", 
	      XDisplayName(display_name)); 
      exit (1); 
    } 
  screen= DefaultScreen(dpy);
  Root = RootWindow(dpy, screen);
  d_depth = DefaultDepth(dpy, screen);
  x_fd = XConnectionNumber(dpy);
  
  /* Icon Daten nach XImage konvertieren */
  GetXPM();
  
  /* Create a window to hold the banner */
  mysizehints.flags= USSize|USPosition;
  mysizehints.x = 0;
  mysizehints.y = 0;

  back_pix = GetColor("white");
  fore_pix = GetColor("black");

  XWMGeometry(dpy, screen, Geometry, NULL, (borderwidth =1), &mysizehints,
	      &mysizehints.x,&mysizehints.y,&mysizehints.width,&mysizehints.height, &i); 

  mysizehints.width = wsclock.attributes.width;
  mysizehints.height= wsclock.attributes.height;

  win = XCreateSimpleWindow(dpy,Root,mysizehints.x,mysizehints.y,
			    mysizehints.width,mysizehints.height,
			    borderwidth,fore_pix,back_pix);
  iconwin = XCreateSimpleWindow(dpy,win,mysizehints.x,mysizehints.y,
				mysizehints.width,mysizehints.height,
				borderwidth,fore_pix,back_pix);

  wm_delete_window = XInternAtom (dpy, 
                                  "WM_DELETE_WINDOW",
                                  False);
  (void) XSetWMProtocols (dpy, win,
                          &wm_delete_window, 1);


  /* Hints aktivieren */
  XSetWMNormalHints(dpy, win, &mysizehints);
  classHint.res_name =  "wsclock";
  classHint.res_class = "ASClock";
  XSetClassHint(dpy, win, &classHint);

  XSelectInput(dpy,win,MW_EVENTS);
  if (hadArguments)
     XSelectInput(dpy,iconwin,MW_EVENTS);
  else
     XSelectInput(dpy,iconwin,MW_EVENTS&~ButtonPressMask);
  
  if (XStringListToTextProperty(&wname, 1, &name) ==0) {
    fprintf(stderr, "wsclock: can't allocate window name\n");
    exit(-1);
  }
  XSetWMName(dpy, win, &name);
  
  /* Create a GC for drawing */
  gcm = GCForeground|GCBackground|GCGraphicsExposures;
  gcv.foreground = fore_pix;
  gcv.background = back_pix;
  gcv.graphics_exposures = FALSE;
  NormalGC = XCreateGC(dpy, Root, gcm, &gcv);  

  /* if (ONLYSHAPE) { try to make shaped window here */
    XShapeCombineMask(dpy, win, ShapeBounding, 0, 0, wsclock.mask, ShapeSet);
    XShapeCombineMask(dpy, iconwin, ShapeBounding, 0, 0, wsclock.mask, ShapeSet);
  
  
  mywmhints.initial_state = (itdocks ? WithdrawnState
			     : (ICONIFIED ? IconicState : NormalState));
  mywmhints.icon_window = iconwin;
  mywmhints.icon_x = mysizehints.x;
  mywmhints.icon_y = mysizehints.y;
  mywmhints.flags = StateHint | IconWindowHint | IconPositionHint;
  if (itdocks) {
    mywmhints.window_group = win;
    mywmhints.flags |= WindowGroupHint;
  }
  XSetWMHints(dpy, win, &mywmhints);
    
  if (itdocks)
    XSetCommand(dpy, win, argv, argc);

  XMapWindow(dpy,win);

  InsertTime();
  RedrawWindow(&visible);
  while(1)
    {
      if (actualtime != mytime())
	  {
	  actualtime = mytime();
	  if(actualmin != actualtime / 60)
	    	{
	      		InsertTime();
	      		if (!itblinks) RedrawWindow(&visible);
	    	}

	  if (led_visible)
	    if( itblinks && (! YEAR))
	      {  
		if (actualtime % 2)
		  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
			    10*led_elem_width, 0,
			    (led_elem_width+1)/2, led_elem_height, 
			    ((showampm!=0) ? led_12h_colon_x : led_24h_colon_x), led_12h_topy);
		
	    	else
		  /* Sekunden Doppelpunkt aus */
		  XCopyArea(dpy, wsclock.pixmap, visible.pixmap, NormalGC,
			    ((showampm!=0) ? led_12h_colon_x : led_24h_colon_x), led_12h_topy,
			    (led_elem_width+1)/2, led_elem_height, 
			    ((showampm!=0) ? led_12h_colon_x : led_24h_colon_x), led_12h_topy);
	  	RedrawWindow(&visible);
	      }
	  }
      
      /* read a packet */
      while (XPending(dpy))
	{
	  XNextEvent(dpy,&Event);
	  switch(Event.type)
	    {
	    case Expose:
	      if(Event.xexpose.count == 0 )
		RedrawWindow(&visible);
	      break;
	    case ButtonPress:
	      system(exec_str);
	      break;
	    case DestroyNotify:
	      /*
		XFreeGC(dpy, NormalGC);
		XDestroyWindow(dpy, win);
		XDestroyWindow(dpy, iconwin);
	      */
	      XCloseDisplay(dpy);
	      exit(0); 
	    case ClientMessage:
	      {
		if( Event.xclient.data.l[0] == wm_delete_window)
		  {
		    XCloseDisplay(dpy);
		    exit(0);
		  }
	      }
	      break;
	    default:
	      break;      
	    }
	}
      XFlush(dpy);
#ifdef SYSV
      if (YEAR) {
	poll((struct poll *) 0, (size_t) 0, 200);
      } else
	poll((struct poll *) 0, (size_t) 0, 50);
#else
      if (YEAR) {
	usleep(200000L);               /* 1/5 sec */
      } else
	usleep(50000L);			/* 5/100 sec */
#endif
    }
  return 0;
}
/****************************************************************************/
void nocolor(char *a, char *b)
{
 fprintf(stderr,"wsclock: can't %s %s\n", a,b);
}
/****************************************************************************/
/* Konvertiere XPMIcons nach XImage */
void GetXPM(void)
{
  XColor col;
  XWindowAttributes attributes;
  int ret;

  /* for the colormap */
  XGetWindowAttributes(dpy,Root,&attributes);


  /* ---------------------------------------------------------------- */

  wsclock.attributes.closeness = 40000; /* Allow for "similar" colors */
  wsclock.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions | XpmCloseness);

  if (clock_xpm_fn[0])
    ret = XpmReadFileToPixmap(dpy, Root, clock_xpm_fn,
			      &wsclock.pixmap, &wsclock.mask, &wsclock.attributes);
  else 
    ret = XpmCreatePixmapFromData(dpy, Root, clock_xpm, &wsclock.pixmap, 
				  &wsclock.mask, &wsclock.attributes);
 
  if(ret != XpmSuccess)
    {nocolor("create wsclock xpm", ERR_colorcells);exit(1);}
  
  /* ---------------------------------------------------------------- */


  visible.attributes.closeness = 40000; /* Allow for "similar" colors */
  visible.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions | XpmCloseness);

 if (clock_xpm_fn[0])
  ret = XpmReadFileToPixmap(dpy, Root, clock_xpm_fn, &visible.pixmap, 
				&visible.mask, &visible.attributes);
 else
  ret = XpmCreatePixmapFromData(dpy, Root, clock_xpm, &visible.pixmap, 
				&visible.mask, &visible.attributes);
 if(ret != XpmSuccess)
   {nocolor("create visible xpm", ERR_colorcells);exit(1);}


  /* ---------------------------------------------------------------- */


  led.attributes.closeness = 40000; /* Allow for "similar" colors */
  led.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions | XpmCloseness);

  if (led_visible && led_xpm_fn[0])
    ret = XpmReadFileToPixmap(dpy, Root, led_xpm_fn, &led.pixmap, 
			      &led.mask, &led.attributes);
  else
    ret = XpmCreatePixmapFromData(dpy, Root, led_xpm, &led.pixmap, 
				  &led.mask, &led.attributes);
  if(ret != XpmSuccess)
    {nocolor("create led xpm", ERR_colorcells);exit(1);}


  /* ---------------------------------------------------------------- */


  month.attributes.closeness = 40000; /* Allow for "similar" colors */
  month.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions | XpmCloseness);

  if (month_visible && month_xpm_fn[0])
    ret = XpmReadFileToPixmap(dpy, Root, month_xpm_fn,
			      &month.pixmap, &month.mask, &month.attributes);
  else 
    ret = XpmCreatePixmapFromData(dpy, Root, month_xpm, &month.pixmap, 
				  &month.mask, &month.attributes);
  if(ret != XpmSuccess)
    {nocolor("create month xpm", ERR_colorcells);exit(1);}



  /* ---------------------------------------------------------------- */


  date.attributes.closeness = 40000; /* Allow for "similar" colors */
  date.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions | XpmCloseness);

  if (cal_visible && date_xpm_fn[0]) 
    ret = XpmReadFileToPixmap(dpy, Root, date_xpm_fn, &date.pixmap, 
			      &date.mask, &date.attributes);
  else
    ret = XpmCreatePixmapFromData(dpy, Root, date_xpm, &date.pixmap, 
				  &date.mask, &date.attributes);
  if(ret != XpmSuccess)
    {nocolor("create date xpm", ERR_colorcells);exit(1);}


  /* ---------------------------------------------------------------- */


  weekday.attributes.closeness = 40000; /* Allow for "similar" colors */
  weekday.attributes.valuemask |= (XpmReturnPixels | XpmReturnExtensions | XpmCloseness);

  if (week_visible && weekday_xpm_fn[0]) 
    ret = XpmReadFileToPixmap(dpy, Root, weekday_xpm_fn,
			      &weekday.pixmap, &weekday.mask, &weekday.attributes);
  else
    ret = XpmCreatePixmapFromData(dpy, Root, weekday_xpm, &weekday.pixmap,
				  &weekday.mask, &weekday.attributes);
  if(ret != XpmSuccess)
    {nocolor("create weekday xpm", ERR_colorcells);exit(1);}

}
/****************************************************************************/
/* Removes expose events for a specific window from the queue */
int flush_expose (Window w)
{
  XEvent dummy;
  int i=0;
  
  while (XCheckTypedWindowEvent (dpy, w, Expose, &dummy))i++;
  return i;
}

/****************************************************************************/
/* Draws the icon window */
void RedrawWindow( XpmIcon *v)
{
  flush_expose (iconwin);
  XCopyArea(dpy,v->pixmap,iconwin,NormalGC,
	    0,0,v->attributes.width, v->attributes.height,0,0);
  flush_expose (win);
  XCopyArea(dpy,v->pixmap,win,NormalGC,
	    0,0,v->attributes.width, v->attributes.height,0,0);

}
/****************************************************************************/
Pixel GetColor(char *name)
{
  XColor color;
  XWindowAttributes attributes;

  XGetWindowAttributes(dpy,Root,&attributes);
  color.pixel = 0;
   if (!XParseColor (dpy, attributes.colormap, name, &color)) 
     {
       nocolor("parse",name);
     }
   else if(!XAllocColor (dpy, attributes.colormap, &color)) 
     {
       nocolor("alloc",name);
     }
  return color.pixel;
}

/****************************************************************************/
static struct tm *clk;
void Year()
{
  int year;

  year = clk->tm_year + 1900;

  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (year / 1000),0, 
	    led_elem_width,led_elem_height,
	    led_24h_hour_x1, led_24h_topy);
  
  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * ((year % 1000) / 100),0, 
	    led_elem_width,led_elem_height,
	    led_24h_hour_x1+led_elem_width+1, led_24h_topy);
  
  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * ((year % 100) / 10),0, 
	    led_elem_width,led_elem_height,
	    led_24h_hour_x1+(led_elem_width+1)*2, led_24h_topy);
  
  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (year % 10),0, 
	    led_elem_width,led_elem_height,
	    led_24h_hour_x1+(led_elem_width+1)*3, led_24h_topy);
}

void Twelve()
{
  int thishour;
  /* Stunde ohne am/pm */
  thishour = clk->tm_hour % 12;
  if (thishour == 0 )
    thishour = 12;

  /* the hour */
  if (thishour>9)
    XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	      led_elem_width, 0, 
	      led_elem_width, led_elem_height, 
	      led_12h_hour_x1, led_12h_topy);

  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (thishour % 10), 0, 
	    led_elem_width, led_elem_height,
	    led_12h_hour_x2, led_12h_topy);
  
  /* the minute */
  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (clk->tm_min / 10),  0,  
	    led_elem_width, led_elem_height, 
	    led_12h_min_x1, led_12h_topy);

  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (clk->tm_min % 10), 0, 
	    led_elem_width, led_elem_height, 
	    led_12h_min_x2, led_12h_topy);
  if (clk->tm_hour >= 12)
    {
      /* PM */
      XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
		11*led_elem_width+led_ampm_width, 0, 
		led_ampm_width, led_elem_height, 
		led_ampm_x, led_12h_topy);
    }
  else
    /* AM */
    XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	      11*led_elem_width, 0, 
	      led_ampm_width, led_elem_height, 
	      led_ampm_x, led_12h_topy);
  
}

void TwentyFour()
{
  /* the hour */
  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (clk->tm_hour / 10), 0, 
	    led_elem_width, led_elem_height, 
	    led_24h_hour_x1, led_24h_topy);

  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (clk->tm_hour % 10), 0, 
	    led_elem_width, led_elem_height, 
	    led_24h_hour_x2, led_24h_topy);

  /* Minute */
  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (clk->tm_min / 10), 0, 
	    led_elem_width, led_elem_height, 
	    led_24h_min_x1, led_24h_topy);

  XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	    led_elem_width * (clk->tm_min % 10), 0, 
	    led_elem_width, led_elem_height, 
	    led_24h_min_x2, led_24h_topy);
  
}
/****************************************************************************/

int mytime()
{
  struct timeval tv;
  struct timezone tz;

  gettimeofday(&tv, &tz);

  return tv.tv_sec;
}
/****************************************************************************/
void InsertTime()
{
  int thismonth, thisweekday, thisdate;

  /* Zeit auslesen */
  actualtime = mytime();
  actualmin = actualtime / 60;

  clk = localtime(&actualtime);

  /* leere wsclock holen */
  XCopyArea(dpy, wsclock.pixmap, visible.pixmap, NormalGC,
	    0,0,mysizehints.width,mysizehints.height,0,0);

  if( led_visible )
    {
      if (YEAR) {
	Year();
      } else if (showampm)
	{
	  Twelve();
	}
      else
	TwentyFour();
    }
  
  /* Month */ 
  if( month_visible )
    XCopyArea(dpy, month.pixmap, visible.pixmap, NormalGC,
	      0, month_elem_height*(clk->tm_mon ), 
	      month_elem_width, month_elem_height, 
	      month_x, month_y);

  /* Date */
  if( cal_visible ) {
    if (clk->tm_mday>9)
      {
	XCopyArea(dpy, date.pixmap, visible.pixmap, NormalGC,
		  cal_elem_width * ((clk->tm_mday / 10 +9) % 10), 0, 
		  cal_elem_width, cal_elem_height, 
		  cal_x1, cal_topy);
	
	XCopyArea(dpy, date.pixmap, visible.pixmap, NormalGC,
		  cal_elem_width * ((clk->tm_mday % 10 +9) % 10), 0, 
		  cal_elem_width, cal_elem_height, 
		  cal_x2, cal_topy);
      }
    else
      XCopyArea(dpy, date.pixmap, visible.pixmap, NormalGC,
		cal_elem_width * (clk->tm_mday -1), 0, 
		cal_elem_width, cal_elem_height, 
		cal_x, cal_topy);
  }

  /* Wochentag */
  if (week_visible)
    XCopyArea(dpy, weekday.pixmap, visible.pixmap, NormalGC,
	      0, week_elem_height * ((clk->tm_wday +6) % 7), 
	      week_elem_width, week_elem_height, 
	      week_x, week_y); 
  
  if (led_visible && (! itblinks) && (! YEAR)) 
    /* Sekunden Doppelpunkt ein */
    XCopyArea(dpy, led.pixmap, visible.pixmap, NormalGC,
	      10*led_elem_width, 0,
	      (led_elem_width+1)/2, led_elem_height, 
	      (showampm!=0 ? led_12h_colon_x : led_24h_colon_x), led_12h_topy);
  
}












