/* Expert.c- expert user options
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

#include "WPrefs.h"

/* This structure containts the list of all the check-buttons to display in the
 * expert tab of the window with the corresponding information for effect
 */
static const struct {
	const char *label;  /* Text displayed to user */

	int def_state;  /* True/False: the default value, if not defined in current config */

	enum {
		OPTION_WORKSPACE,
		OPTION_USERDEF
	} class;

	const char *op_name;  /* The identifier for the option in the config file */

} expert_options[] = {

	{ N_("Disable miniwindows (icons for minimized windows). For use with KDE/GNOME."),
	  /* default: */ False, OPTION_WORKSPACE, "DisableMiniwindows" },

	{ N_("Do not set non-Workspace specific parameters (do not use xset)."),
	  /* default: */ False, OPTION_USERDEF, "NoXSetStuff" },

	{ N_("Automatically save session when exiting Workspace."),
	  /* default: */ False, OPTION_WORKSPACE, "SaveSessionOnExit" },

	{ N_("Use SaveUnder in window frames, icons, menus and other objects."),
	  /* default: */ False, OPTION_WORKSPACE, "UseSaveUnders" },

	{ N_("Disable confirmation panel for the Kill command."),
	  /* default: */ False, OPTION_WORKSPACE, "DontConfirmKill" },

	{ N_("Disable selection animation for selected icons."),
	  /* default: */ False, OPTION_WORKSPACE, "DisableBlinking" },

	{ N_("Smooth font edges (needs restart)."),
	  /* default: */ True, OPTION_WORKSPACE, "AntialiasedText" },

	{ N_("Cycle windows only on the active head."),
	  /* default: */ False, OPTION_WORKSPACE, "CycleActiveHeadOnly" },

	{ N_("Ignore minimized windows when cycling."),
	  /* default: */ False, OPTION_WORKSPACE, "CycleIgnoreMinimized" },

	{ N_("Show workspace title on Clip."),
	  /* default: */ True, OPTION_WORKSPACE, "ShowClipTitle" },

	{ N_("Highlight the icon of the application when it has the focus."),
	  /* default: */ True, OPTION_WORKSPACE, "HighlightActiveApp" },

#ifdef XKB_MODELOCK
	{ N_("Enable keyboard language switch button in window titlebars."),
	  /* default: */ False, OPTION_WORKSPACE, "KbdModeLock" }
#endif /* XKB_MODELOCK */

};


typedef struct _Panel {
	WMBox *box;
	char *sectionName;

	char *description;

	CallbackRec callbacks;

	WMWidget *parent;

	WMButton *swi[sizeof(expert_options) / sizeof(expert_options[0])];

} _Panel;

#define ICON_FILE	"expert"


static void createPanel(Panel * p)
{
	_Panel *panel = (_Panel *) p;
	WMScrollView *sv;
	WMFrame *f;
	WMUserDefaults *udb;
	int i, state;

	panel->box = WMCreateBox(panel->parent);
	WMSetViewExpandsToParent(WMWidgetView(panel->box), 2, 2, 2, 2);

	sv = WMCreateScrollView(panel->box);
	WMResizeWidget(sv, 500, 215);
	WMMoveWidget(sv, 12, 10);
	WMSetScrollViewRelief(sv, WRSunken);
	WMSetScrollViewHasVerticalScroller(sv, True);
	WMSetScrollViewHasHorizontalScroller(sv, False);

	f = WMCreateFrame(panel->box);
	WMResizeWidget(f, 495, (sizeof(expert_options) / sizeof(expert_options[0])) * 25 + 8);
	WMSetFrameRelief(f, WRFlat);

	udb = WMGetStandardUserDefaults();
	for (i = 0; i < sizeof(expert_options) / sizeof(expert_options[0]); i++) {
		panel->swi[i] = WMCreateSwitchButton(f);
		WMResizeWidget(panel->swi[i], FRAME_WIDTH - 40, 25);
		WMMoveWidget(panel->swi[i], 5, 5 + i * 25);

		WMSetButtonText(panel->swi[i], _(expert_options[i].label));

		switch (expert_options[i].class) {
		case OPTION_WORKSPACE:
			if (GetStringForKey(expert_options[i].op_name))
				state = GetBoolForKey(expert_options[i].op_name);
			else
				state = expert_options[i].def_state;
			break;

		case OPTION_USERDEF:
			state = WMGetUDBoolForKey(udb, expert_options[i].op_name);
			break;

		}
		WMSetButtonSelected(panel->swi[i], state);
	}

	WMMapSubwidgets(panel->box);
	WMSetScrollViewContentView(sv, WMWidgetView(f));
	WMRealizeWidget(panel->box);
}

static void storeDefaults(_Panel * panel)
{
	WMUserDefaults *udb = WMGetStandardUserDefaults();
	int i;

	for (i = 0; i < sizeof(expert_options) / sizeof(expert_options[0]); i++) {
		switch (expert_options[i].class) {
		case OPTION_WORKSPACE:
			SetBoolForKey(WMGetButtonSelected(panel->swi[i]), expert_options[i].op_name);
			break;

		case OPTION_USERDEF:
			WMSetUDBoolForKey(udb, WMGetButtonSelected(panel->swi[i]), expert_options[i].op_name);
			break;
		}
	}
}

Panel *InitExpert(WMScreen * scr, WMWidget * parent)
{
	_Panel *panel;

	panel = wmalloc(sizeof(_Panel));

	panel->sectionName = _("Expert User Preferences");

	panel->description = _("Options for people who know what they're doing...\n"
			       "Also has some other misc. options.");

	panel->parent = parent;

	panel->callbacks.createWidgets = createPanel;
	panel->callbacks.updateDomain = storeDefaults;

	AddSection(panel, ICON_FILE);

	return panel;
}
