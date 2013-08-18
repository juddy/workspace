#ifndef ASCLOCK_H
#define ASCLOCK_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <X11/extensions/shape.h>
#include <X11/Xatom.h>

extern char version_id[];

#define TRUE 1
#define FALSE 0
typedef struct _symbol symbol;

struct _symbol
{
  char *name;
  int *addr;
};
extern symbol config_symbols[];

#define MAX_PATH_LEN 512
#define INT_TYPE 0;

/* the xpm data */
extern char *clock_xpm[];
extern char *month_xpm[];
extern char *weekday_xpm[];
extern char *led_xpm[];
extern char *date_xpm[];
extern char *hour_xpm[];
extern char *minute_xpm[];
extern char *second_xpm[];

/* the filenames */
extern char clock_xpm_fn[MAX_PATH_LEN];
extern char month_xpm_fn[MAX_PATH_LEN];
extern char weekday_xpm_fn[MAX_PATH_LEN];
extern char led_xpm_fn[MAX_PATH_LEN];
extern char date_xpm_fn[MAX_PATH_LEN];
extern char hour_xpm_fn[MAX_PATH_LEN];
extern char min_xpm_fn[MAX_PATH_LEN];
extern char sec_xpm_fn[MAX_PATH_LEN];

extern char exec_str[MAX_PATH_LEN];

extern int itblinks;
extern int showampm;
extern int itdocks;

extern int led_visible;
extern int led_elem_width;
extern int led_elem_height;
extern int led_12h_hour_x1;
extern int led_12h_hour_x2;
extern int led_12h_colon_x;
extern int led_12h_min_x1;
extern int led_12h_min_x2;
extern int led_ampm_x;
extern int led_ampm_width;
extern int led_12h_topy;
extern int led_24h_hour_x1;
extern int led_24h_hour_x2;
extern int led_24h_colon_x;
extern int led_24h_min_x1;
extern int led_24h_min_x2;
extern int led_24h_topy;

extern int week_visible;
extern int week_elem_width;
extern int week_elem_height;
extern int week_x;
extern int week_y;

extern int cal_visible;
extern int cal_elem_width;
extern int cal_elem_height;
extern int cal_x;
extern int cal_x1;
extern int cal_x2;
extern int cal_topy;

extern int month_visible;
extern int month_elem_width;
extern int month_elem_height;
extern int month_x;
extern int month_y;

extern int analog_visible;
extern int hour_visible ;
extern int hour_center_x ;
extern int hour_center_y ;
extern int hour_rot_x ;
extern int hour_rot_y ;

extern int min_visible ;
extern int min_center_x ;
extern int min_center_y ;
extern int min_rot_x ;
extern int min_rot_y ;

extern int sec_visible ;
extern int sec_center_x ;
extern int sec_center_y ;
extern int sec_rot_x ;
extern int sec_rot_y ;

extern char *hour_map;
extern char *min_map;
extern char *sec_map;

/********* config.c ****************/

void config();
void parseArgs(int argc, char **argv);

/********* parser.c ****************/

int read_init(FILE *f);
int read_type(int *type);
int read_token(char *str, int max);
int read_assign();
int read_int(int *ret);
int read_semicolon();

#endif /* ASCLOCK_H */






