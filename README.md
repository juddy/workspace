# Workspace

This is 'Workspace' a fork of Window Maker.

NeXTSTEP is dead, the 90's are over, and in Real Estate jargon, Window Maker has good bones:

- Multiple workspaces
- Heavily keyboard-operable (window sizing, desktop/app switching, command execution, etc.)
- Apart from some of the rigidities of the Dock/Clip, it's pretty easy to make Window Maker bend to your whims.
- Small memory footprint
- WINGs, wmlib, wrlib, libc - No shenanigans with GNOMEs, no Krap.
- It can just create windows (without borders and titlebars, sunken and unclickable if you wish) or give you a rich 'Desktop'.

Workspace is an attempt to step away from the adherence to the NeXTisms of yester-decade and embrace some other areas of UI/UX:

- Subversion of the Desktop paradigm
- Lean, with a focus on Terminal and VIM
- Tiled window management
- Mouselessness
- Generic and configurable


## Workspace Keys

Workspace uses the standard WindowMaker keybindings. Set in Workspace/Defaults/WindowMaker.in and rebuild, or manage a subset of them with WPrefs or xetosconf.

ModifierKey = Mod1 (Alt)

CloseKey = None

MaximizeKey = None

VMaximizeKey = None

RaiseLowerKey = None

ShadeKey = None

NextWorkspaceKey = "Mod1+Control+Right"

PrevWorkspaceKey = "Mod1+Control+Left"

RootMenuKey = F12

WindowListKey = F11

WindowMenuKey = "Control+Escape"

RaiseKey = "Mod1+Up"

LowerKey = "Mod1+Down"

FocusNextKey = "Mod1+Tab"

FocusPrevKey = "Mod1+Shift+Tab"

Workspace1Key = "Mod1+1"

Workspace2Key = "Mod1+2"

Workspace3Key = "Mod1+3"

Workspace4Key = "Mod1+4"

Workspace5Key = "Mod1+5"

Workspace6Key = "Mod1+6"

Workspace7Key = "Mod1+7"

Workspace8Key = "Mod1+8"

Workspace9Key = "Mod1+9"

Workspace10Key = "Mod1+0"

HideKey = "Mod1+H"

SelectKey = None

MiniaturizeKey = "Mod1+M"

NextWorkspaceLayerKey = None

PrevWorkspaceLayerKey = None

DockRaiseLowerKey = None

ClipRaiseLowerKey = None


## Installation

A standard './configure && make && sudo make install' should do, with the exception of Clock, which may require Makefile re-dos with 'xmkmf' (it's old code, for sure).

You may choose to 'make install', and run 'setup.sh' to step through the Autotools cleanup/rebuild steps.

Binaries are present, and you may be able to get by (on Debian "Jessie") by running 'make install'.

## Execution

Workspace can be started from .xinitc or by setting your default windowmanager to use '$(which) workspace':

  cd /etc/defaults/ ; ln -s $(which workspace) /etc/default/windowmanager

## To Do

- Default Theme/Style enhancements
- Better integration with xmux, xtab (tmux, tabbed)
- Remove legacy UNIX cruft. SCO, anyone?
- Enhance smart maximization code to bring about 'tiling', share with wmaker.