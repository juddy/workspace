
defsdatadir = $(sysconfdir)/Workspace

defsdata_DATA = WMRootMenu WMWindowAttributes Workspace WMState WMGLOBAL

wpexecbindir = @wprefs_bindir@

EXTRA_DIST = WMGLOBAL WMWindowAttributes.in Workspace.in WMState.in

CLEANFILES = WMWindowAttributes Workspace WMState WMRootMenu

WMWindowAttributes: $(srcdir)/WMWindowAttributes.in
	-rm -f WMWindowAttributes
	sed -e "s:#extension#:@ICONEXT@:" \
		$(srcdir)/WMWindowAttributes.in > WMWindowAttributes
	chmod 644 WMWindowAttributes

Workspace: $(srcdir)/Workspace.in
	-rm -f Workspace
	sed -e "s:#pkgdatadir#:$(pkgdatadir):" $(srcdir)/Workspace.in \
		> Workspace
	chmod 644 Workspace

WMState: $(srcdir)/WMState.in
	-rm -f WMState
	sed -e "s:#wprefs#:$(wpexecbindir)/WPrefs:" $(srcdir)/WMState.in > WMState
	chmod 644 WMState

WMRootMenu: $(srcdir)/../plmenu
	-cp -f $(srcdir)/../plmenu WMRootMenu

