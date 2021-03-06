/* WPrefs.h- general definitions
 *
 *  WPrefs - Workspace Preferences Program
 *
 *  Copyright (c) 1998-2003 Alfredo K. Kojima
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef WPREFS_H_
#define WPREFS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <signal.h>

#include <X11/Xlib.h>

#include <wraster.h>

#include <WINGs/WINGs.h>

/* Needed for HAVE_LIBINTL_H and EXTENDED_WINDOWSHORTCUT */
#include "../src/wconfig.h"

/****/

extern char *NOptionValueChanged;
extern Bool xext_xkb_supported;

typedef struct _Panel Panel;

typedef struct {
    unsigned flags;		       /* reserved for WPrefs.c Don't access it */

    void (*createWidgets)(Panel*);     /* called when showing for first time */
    void (*updateDomain)(Panel*);      /* save the changes to the dictionary */
    Bool (*requiresRestart)(Panel*);   /* return True if some static option was changed */
    void (*undoChanges)(Panel*);       /* reset values to those in the dictionary */
    void (*prepareForClose)(Panel*);   /* called when exiting WPrefs */
    void (*showPanel)(Panel*);	       /* called when entering the panel */
    void (*hidePanel)(Panel*);	       /* called when exiting the panel */
} CallbackRec;


/* all Panels must start with the following layout */
typedef struct PanelRec {
    WMBox *box;

    char *sectionName;		       /* section name to display in titlebar */

    char *description;

    CallbackRec callbacks;
} PanelRec;


/* ---[ Wprefs.c ] ------------------------------------------------------- */

void AddSection(Panel *panel, const char *iconFile);

char *LocateImage(const char *name);

void SetButtonAlphaImage(WMScreen *scr, WMButton *bPtr, const char *file,
                         const char *title1, const char *title2);

/* Loads `file' into `icon_normal'. If `icon_greyed' is not NULL,
 * combine `icon_normal' with some grey and then optionally with image
 * `xis', and store it in `icon_greyed' (typically to produce a
 * greyed-out, red-crossed version of `icon_normal') */
void CreateImages(WMScreen *scr, RContext *rc, RImage *xis, const char *file,
		WMPixmap **icon_normal, WMPixmap **icon_greyed);

WMWindow *GetWindow(Panel *panel);

/* manipulate the dictionary for the Workspace domain */

WMPropList *GetObjectForKey(const char *defaultName);

void SetObjectForKey(WMPropList *object, const char *defaultName);

void RemoveObjectForKey(const char *defaultName);

char *GetStringForKey(const char *defaultName);

int GetIntegerForKey(const char *defaultName);

Bool GetBoolForKey(const char *defaultName);

int GetSpeedForKey(const char *defaultName);

void SetIntegerForKey(int value, const char *defaultName);

void SetStringForKey(const char *value, const char *defaultName);

void SetBoolForKey(Bool value, const char *defaultName);

void SetSpeedForKey(int speed, const char *defaultName);


/* ---[ KeyboardShortcuts.c ] -------------------------------------------- */

char *capture_shortcut(Display *dpy, Bool *capturing, Bool convert_case);


/* ---[ double.c ] ------------------------------------------------------- */
typedef struct W_DoubleTest DoubleTest;

DoubleTest *CreateDoubleTest(WMWidget *parent, const char *text);


/* ---[ main.c ] --------------------------------------------------------- */
void AddDeadChildHandler(pid_t pid, void (*handler)(void*), void *data);


/* ---[ xmodifier.c ] ---------------------------------------------------- */
int ModifierFromKey(Display * dpy, const char *key);


/* ---[ Panel Initializers ]---------------------------------------------- */

void Initialize(WMScreen *scr);

/* in alphabetical order - in case you'd want to add one */
Panel *InitAppearance(WMScreen *scr, WMWidget *parent);
Panel *InitConfigurations(WMScreen *scr, WMWidget *parent);
Panel *InitDocks(WMScreen *scr, WMWidget *parent);
Panel *InitExpert(WMScreen *scr, WMWidget *parent);
Panel *InitFocus(WMScreen *scr, WMWidget *parent);
Panel *InitFontSimple(WMScreen *scr, WMWidget *parent);
Panel *InitIcons(WMScreen *scr, WMWidget *parent);
Panel *InitKeyboardShortcuts(WMScreen *scr, WMWidget *parent);
Panel *InitMenu(WMScreen *scr, WMWidget *parent);
Panel *InitMenuPreferences(WMScreen *scr, WMWidget *parent);
Panel *InitMouseSettings(WMScreen *scr, WMWidget *parent);
Panel *InitPaths(WMScreen *scr, WMWidget *parent);
Panel *InitPreferences(WMScreen *scr, WMWidget *parent);
Panel *InitWindowHandling(WMScreen *scr, WMWidget *parent);
Panel *InitWorkspace(WMScreen *scr, WMWidget *parent);


#define FRAME_TOP	105
#define FRAME_LEFT	-2
#define FRAME_WIDTH	524
#define FRAME_HEIGHT	235
#endif /* WPREFS_H_ */

