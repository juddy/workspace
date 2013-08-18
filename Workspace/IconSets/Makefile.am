
defsdatadir = $(pkgdatadir)/IconSets

defsdata_DATA = Default.iconset

EXTRA_DIST = 

CLEANFILES = Default.iconset

Default.iconset: $(top_builddir)/Workspace/Defaults/WMWindowAttributes
	-rm -f Default.iconset
	echo '{' > Default.iconset
	grep Icon $(top_builddir)/Workspace/Defaults/WMWindowAttributes >> Default.iconset
	echo '}' >> Default.iconset
	chmod 644 Default.iconset


