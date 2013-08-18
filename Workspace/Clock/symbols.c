#include "wsclock.h"

symbol config_symbols[] = {
  { "led_visible",      &led_visible},
  { "led_elem_width",   &led_elem_width},
  { "led_elem_height",  &led_elem_height},
  { "led_12h_hour_x1",  &led_12h_hour_x1},
  { "led_12h_hour_x2",  &led_12h_hour_x2},
  { "led_12h_colon_x",  &led_12h_colon_x},
  { "led_12h_min_x1",   &led_12h_min_x1},
  { "led_12h_min_x2",   &led_12h_min_x2},
  { "led_ampm_x",       &led_ampm_x},
  { "led_ampm_width",   &led_ampm_width},
  { "led_12h_topy",     &led_12h_topy},
  { "led_24h_hour_x1",  &led_24h_hour_x1},
  { "led_24h_hour_x2",  &led_24h_hour_x2},
  { "led_24h_colon_x",  &led_24h_colon_x},
  { "led_24h_min_x1",   &led_24h_min_x1},
  { "led_24h_min_x2",   &led_24h_min_x2},
  { "led_24h_topy",     &led_24h_topy},
  { "week_visible",     &week_visible},
  { "week_elem_width",  &week_elem_width},
  { "week_elem_height", &week_elem_height},
  { "week_x",           &week_x},
  { "week_y",           &week_y},
  { "cal_visible",      &cal_visible},
  { "cal_elem_width",   &cal_elem_width},
  { "cal_elem_height",  &cal_elem_height},
  { "cal_x1",           &cal_x1},
  { "cal_x2",           &cal_x2},
  { "cal_topy",         &cal_topy},
  { "month_visible",    &month_visible},
  { "month_elem_width", &month_elem_width},
  { "month_elem_height",&month_elem_height},
  { "month_x",          &month_x},
  { "month_y",          &month_y},
  { "analog_visible",   &analog_visible},
  { "hour_visible",     &hour_visible},
  { "hour_center_x",    &hour_center_x},
  { "hour_center_y",    &hour_center_y},
  { "hour_rot_x",       &hour_rot_x},
  { "hour_rot_y",       &hour_rot_y},
  { "min_visible",      &min_visible},
  { "min_center_x",     &min_center_x},
  { "min_center_y",     &min_center_y},
  { "min_rot_x",        &min_rot_x},
  { "min_rot_y",        &min_rot_y},
  { "sec_visible",      &sec_visible},
  { "sec_center_x",     &sec_center_x},
  { "sec_center_y",     &sec_center_y},
  { "sec_rot_x",        &sec_rot_x},
  { "sec_rot_y",        &sec_rot_y},
  { NULL, NULL} };

int itblinks;
int showampm;

int led_visible;
int led_elem_width;
int led_elem_height;
int led_12h_hour_x1;
int led_12h_hour_x2;
int led_12h_colon_x;
int led_12h_min_x1;
int led_12h_min_x2;
int led_ampm_x;
int led_ampm_width;
int led_12h_topy;
int led_24h_hour_x1;
int led_24h_hour_x2;
int led_24h_colon_x;
int led_24h_min_x1;
int led_24h_min_x2;
int led_24h_topy;

int week_visible;
int week_elem_width;
int week_elem_height;
int week_x;
int week_y;

int cal_visible;
int cal_elem_width;
int cal_elem_height;
int cal_x;
int cal_x1;
int cal_x2;
int cal_topy;

int month_visible;
int month_elem_width;
int month_elem_height;
int month_x;
int month_y;

int analog_visible;
int hour_visible ;
int hour_center_x ;
int hour_center_y ;
int hour_rot_x ;
int hour_rot_y ;

int min_visible ;
int min_center_x ;
int min_center_y ;
int min_rot_x ;
int min_rot_y ;

int sec_visible ;
int sec_center_x ;
int sec_center_y ;
int sec_rot_x ;
int sec_rot_y ;

