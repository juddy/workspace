# User-Defined Macros:
# %%define <name> <expansion>
%define Name		wsclock
%define Version		1.0.12.2
%define Release		1
%define Prefix		/usr
%define ExecPrefix	/usr/X11R6
Summary: dockable clock applet for Workspace
Name: %{Name}
Version: %{Version}
Release: %{Release}
#Epoch: 
Copyright: GPL
Group: User Interface/X
URL: http://www.pobox.com/~jmknoble/WindowMaker/wsclock/
Source0: http://www.pobox.com/~jmknoble/WindowMaker/wsclock/%{Name}-%{Version}.tar.gz
#Patch0: 
#Prefix: %{Prefix}
BuildRoot: /tmp/%{Name}-%{Version}-%{Release}-root
#Provides: 
#Requires: 
#Obsoletes: 

%description
Wmclock is an applet which displays the date and time in a dockable
tile in the same style as the clock from the NEXTSTEP(tm) operating
system.  Wmclock is specially designed for the Workspace window
manager, by Alfredo Kojima, and features multiple language support,
twenty-four-hour and twelve-hour (am/pm) time display, and, optionally,
can run a user-specified program on a mouse click.  Wmclock is derived
from asclock, a similar clock for the AfterStep window manager.

%prep
%setup
#%patch0 -b .orig

#function Replace() {
#  local fil="$1"
#  local sep="$2"
#  local old="$3"
#  local new="$4"
#  local suf="$5"
#  [ -z "${suf}" ] && suf='~'
#  mv -f ${fil} ${fil}${suf}
#  cat ${fil}${suf} | sed -e "s${sep}${old}${sep}${new}${sep}g" >$fil
#}

%build
if [ -z "${CC}" ]; then
  if [ -n "`which gcc`" ]; then
    CC="gcc"
  else
    CC="cc"
  fi
fi
export CC

./configure --lang english
make CC="${CC}" CDEBUGFLAGS="${RPM_OPT_FLAGS}"

%install
function CheckBuildRoot() {
  # do a few sanity checks on the BuildRoot
  # to make sure we don't damage a system
  case "${RPM_BUILD_ROOT}" in
    ''|' '|/|/bin|/boot|/dev|/etc|/home|/lib|/mnt|/root|/sbin|/tmp|/usr|/var)
      echo "Yikes!  Don't use '${RPM_BUILD_ROOT}' for a BuildRoot!"
      echo "The BuildRoot gets deleted when this package is rebuilt;"
      echo "something like '/tmp/build-blah' is a better choice."
      return 1
    ;;
    *) return 0
    ;;
  esac
}
function CleanBuildRoot() {
  if CheckBuildRoot; then
    rm -rf "${RPM_BUILD_ROOT}"
  else
    exit 1
  fi
}
CleanBuildRoot

for i in \
  %{Prefix} \
  %{Prefix}/share \
  %{Prefix}/share/%{Name} \
  %{ExecPrefix} \
; do
  mkdir -p "${RPM_BUILD_ROOT}${i}"
done

make DESTDIR="${RPM_BUILD_ROOT}" install
make DESTDIR="${RPM_BUILD_ROOT}" install.man
make DESTDIR="${RPM_BUILD_ROOT}" install.share

%clean
function CheckBuildRoot() {
  # do a few sanity checks on the BuildRoot
  # to make sure we don't damage a system
  case "${RPM_BUILD_ROOT}" in
    ''|' '|/|/bin|/boot|/dev|/etc|/home|/lib|/mnt|/root|/sbin|/tmp|/usr|/var)
      echo "Yikes!  Don't use '${RPM_BUILD_ROOT}' for a BuildRoot!"
      echo "The BuildRoot gets deleted when this package is rebuilt;"
      echo "something like '/tmp/build-blah' is a better choice."
      return 1
    ;;
    *) return 0
    ;;
  esac
}
function CleanBuildRoot() {
  if CheckBuildRoot; then
    rm -rf "${RPM_BUILD_ROOT}"
  else
    exit 1
  fi
}
CleanBuildRoot

%files
%attr(-   ,root,root) %doc COPYING ChangeLog INSTALL README
%attr(0755,root,root) %{ExecPrefix}/bin/wsclock
%attr(0755,root,root) %{ExecPrefix}/man/man1/wsclock.1*
%attr(0755,root,root) %dir %{Prefix}/share/wsclock
%attr(-   ,root,root) %{Prefix}/share/wsclock/*

