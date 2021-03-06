## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS =

BUILT_SOURCES = config-paths.h

DISTCLEANFILES = config-paths.h

config.h: config-paths.h

config-paths.h: Makefile
	@echo "Generating $@"
	@echo '/* this is a generated file - do not edit */' > $@
	@echo '' >> $@
	@echo '/* gettext domain used for menu translations */' >> $@
	@if test -z "$(menutextdomain)"; then \
	  echo '/* #undef MENU_TEXTDOMAIN "$(menutextdomain)" */' >> $@; \
	else \
	  echo '#define MENU_TEXTDOMAIN "$(menutextdomain)"' >> $@; \
	fi
	@echo '' >> $@
	@echo '/* define an extra path for pixmaps */' >> $@
	@echo '#define PIXMAPDIR "$(pixmapdir)"' >> $@
	@echo '' >> $@
	@echo '/* where shared data is stored */' >> $@
	@echo '#define PKGDATADIR "$(datadir)/Workspace"' >> $@
	@echo '' >> $@
	@echo '/* where the configuration is stored */' >> $@
	@echo '#define SYSCONFDIR "$(sysconfdir)"' >> $@

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = wrlib WINGs src util po Workspace wmlib WPrefs.app doc test 

EXTRA_DIST = INSTALL \
	README.definable-cursor \
	README COPYING.WTFPL autogen.sh setup.sh

if USE_LCOV
coverage-reset:
	find . -type f -name '*.gcda' -exec rm -f '{}' ';'
	lcov --directory . --zerocounters

coverage:
	rm -rf coverage @lcov_output_directory@
	-mkdir -p coverage @lcov_output_directory@
	lcov --compat-libtool --capture --directory . --output-file coverage/coverage.info
	genhtml --output-directory @lcov_output_directory@ coverage/coverage.info

.PHONY:	coverage-reset coverage
endif
