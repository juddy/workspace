# Desktop


Current desktop environment - WMaker, tmux, tabbed, vim, Xdefaults

Base directory contains WindowMaker 0.95 with my icon patches.

tmux 1.8 also lives here, as does tabbed, and 'xetosconf', a fork of WmakerConf

Running './configure ; make ; sudo make install' will install WindowMaker alone.

You'll have to install tabbed, tmux, etc separately for now.

## Tabbed Keys

C-Shift-Return	open new tab

C-H		previous tab

C-L		next tab

C-J		move selected tab one to the left

C-K		move selected tab one to the right

C-Tab			toggle between the selected and last selected tab

C-q			close tab

C-[0..9]		jumps to nth tab

F11			Toggle fullscreen mode.

## tmux Keys

C-b         Send the prefix key (C-b) through to the application.

C-o         Rotate the panes in the current window forwards.

C-z         Suspend the tmux client.

!           Break the current pane out of the window.

"           Split the current pane into two, top and bottom.

#           List all paste buffers.

$           Rename the current session.

%           Split the current pane into two, left and right.

&           Kill the current window.

'           Prompt for a window index to select.

,           Rename the current window.

-           Delete the most recently copied buffer of text.

.           Prompt for an index to move the current window.

0 to 9      Select windows 0 to 9.

:           Enter the tmux command prompt.

;           Move to the previously active pane.

=           Choose which buffer to paste interactively from a list.

?           List all key bindings.

D           Choose a client to detach.

[           Enter copy mode to copy text or view the history.

]           Paste the most recently copied buffer of text.

c           Create a new window.

d           Detach the current client.

f           Prompt to search for text in open windows.

i           Display some information about the current window.

l           Move to the previously selected window.

n           Change to the next window.

o           Select the next pane in the current window.

p           Change to the previous window.

q           Briefly display pane indexes.

r           Force redraw of the attached client.

s           Select a new session for the attached client interactively.

L           Switch the attached client back to the last session.

t           Show the time.

w           Choose the current window interactively.

x           Kill the current pane.

{           Swap the current pane with the previous pane.

}           Swap the current pane with the next pane.

~           Show previous messages from tmux, if any.

Page Up     Enter copy mode and scroll one page up.



Up, Down

Left, Right Change to the pane above, below, to the left, or to the right of the current pane.



M-1 to M-5  Arrange panes in one of the five preset layouts: even-horizontal, even-vertical, main-horizontal, main-vertical, or tiled.

M-n         Move to the next window with a bell or activity marker.

M-o         Rotate the panes in the current window backwards.

M-p         Move to the previous window with a bell or activity marker.



C-Up, C-Down

C-Left, C-Right Resize the current pane in steps of one cell.



M-Up, M-Down

M-Left, M-Right Resize the current pane in steps of five cells.



# screen commands

Each screen command consists of a "C-a" followed by one other character.  For your convenience, all commands that are bound  to lower-case  letters  are also bound to their control character counterparts (with the exception of "C-a a"; see below), thus, "C-a c" as well as "C-a C-c" can be used to create a window. See section "CUSTOMIZATION" for a description of the command.





The following table shows the default key bindings:



C-a '       (select)      Prompt for a window name or number to switch to.

C-a "       (windowlist -b) Present a list of all windows for selection.

C-a 0       (select 0)

...           ...

C-a 9       (select 9)

C-a -       (select -)    Switch to window number 0 - 9, or to the blank window.

C-a tab     (focus)       Switch the input focus to the next region.

C-a C-a     (other)       Toggle to the window displayed previously.  Note that this binding defaults to the command character  typed  twice, unless overridden.  For instance, if you use the option "-e]x", this command becomes "]]".

C-a a       (meta)        Send the command character (C-a) to window. See escape command.

C-a A       (title)       Allow the user to enter a name for the current window.

C-a b

C-a C-b     (break)       Send a break to window.

C-a B       (pow_break)   Reopen the terminal line and send a break.

C-a c

C-a C-c     (screen)      Create a new window with a shell and switch to that window.

C-a C       (clear)       Clear the screen.

C-a d

C-a C-d     (detach)      Detach screen from this terminal.

C-a D D     (pow_detach)  Detach and logout.

C-a f

C-a C-f     (flow)        Toggle flow on, off or auto.

C-a F       (fit)         Resize the window to the current region size.

C-a C-g     (vbell)       Toggles screen's visual bell mode.

C-a h       (hardcopy)    Write a hardcopy of the current window to the file "hardcopy.n".

C-a H       (log)         Begins/ends logging of the current window to the file "screenlog.n".

C-a i

C-a C-i     (info)        Show info about this window.

C-a k

C-a C-k     (kill)        Destroy current window.

C-a l

C-a C-l     (redisplay)   Fully refresh current window.

C-a L       (login)       Toggle this windows login slot. Available only if screen is configured to update the utmp database.

C-a m

C-a C-m     (lastmsg)     Repeat the last message displayed in the message line.

C-a M       (monitor)     Toggles monitoring of the current window.

C-a space

C-a n

C-a C-n     (next)        Switch to the next window.

C-a N       (number)      Show the number (and title) of the current window.

C-a backspace

C-a h

C-a p

C-a C-p     (prev)        Switch to the previous window (opposite of C-a n).

C-a q

C-a C-q     (xon)         Send a control-q to the current window.

C-a Q       (only)        Delete all regions but the current one.

C-a r

C-a C-r     (wrap)        Toggle the current window's line-wrap setting (turn the current window's automatic margins on and off).

C-a s

C-a C-s     (xoff)        Send a control-s to the current window.

C-a S       (split)       Split the current region into two new ones.

C-a t

C-a C-t     (time)        Show system information.

C-a v       (version)     Display the version and compilation date.

C-a C-v     (digraph)     Enter digraph.

C-a w

C-a C-w     (windows)     Show a list of window.

C-a W       (width)       Toggle 80/132 columns.

C-a x

C-a C-x     (lockscreen)  Lock this terminal.

C-a X       (remove)      Kill the current region.

C-a z

C-a C-z     (suspend)     Suspend screen.  Your system must support BSD-style job-control.

C-a Z       (reset)       Reset the virtual terminal to its "power-on" values.

C-a .       (dumptermcap) Write out a ".termcap" file.

C-a ?       (help)        Show key bindings.

C-a C-\     (quit)        Kill all windows and terminate screen.

C-a :       (colon)       Enter command line mode.

C-a [

C-a C-[

C-a esc     (copy)        Enter copy/scrollback mode.

C-a ]       (paste .)     Write the contents of the paste buffer to the stdin queue of the current window.

C-a {

C-a }       (history)     Copy and paste a previous (command) line.

C-a >       (writebuf)    Write paste buffer to a file.

C-a <       (readbuf)     Reads the screen-exchange file into the paste buffer.

C-a =       (removebuf)   Removes the file used by C-a < and C-a >.

C-a ,       (license)     Shows where screen comes from, where it went to and why you can use it.

C-a _       (silence)     Start/stop monitoring the current window for inactivity.

C-a *       (displays)    Show a listing of all currently attached displays.

## WindowMaker Keys
WindowMaker default Keybindings. Set in WindowMaker/Defaults/WindowMaker.in 

ModifierKey = Mod1

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
