
#include <unistd.h>
#include <X11/Xlocale.h>

#include "WINGsP.h"
#include "wconfig.h"


struct W_Application WMApplication;

char *_WINGS_progname = NULL;

Bool W_ApplicationInitialized(void)
{
	return _WINGS_progname != NULL;
}

void WMInitializeApplication(const char *applicationName, int *argc, char **argv)
{
	int i;

	assert(argc != NULL);
	assert(argv != NULL);
	assert(applicationName != NULL);

	setlocale(LC_ALL, "");

#ifdef I18N
	if (getenv("NLSPATH"))
		bindtextdomain("WINGs", getenv("NLSPATH"));
	else
		bindtextdomain("WINGs", LOCALEDIR);
	bind_textdomain_codeset("WINGs", "UTF-8");
#endif

	_WINGS_progname = argv[0];

	WMApplication.applicationName = wstrdup(applicationName);
	WMApplication.argc = *argc;

	WMApplication.argv = wmalloc((*argc + 1) * sizeof(char *));
	for (i = 0; i < *argc; i++) {
		WMApplication.argv[i] = wstrdup(argv[i]);
	}
	WMApplication.argv[i] = NULL;

	/* initialize notification center */
	W_InitNotificationCenter();
}

void WMSetResourcePath(const char *path)
{
	if (WMApplication.resourcePath)
		wfree(WMApplication.resourcePath);
	WMApplication.resourcePath = wstrdup(path);
}

char *WMGetApplicationName()
{
	return WMApplication.applicationName;
}

static char *checkFile(const char *path, const char *folder, const char *ext, const char *resource)
{
	char *ret;
	int extralen;
	size_t slen;

	if (!path || !resource)
		return NULL;

	extralen = (ext ? strlen(ext) + 1 : 0) + (folder ? strlen(folder) + 1 : 0) + 1;
	slen = strlen(path) + strlen(resource) + 1 + extralen;
	ret = wmalloc(slen);

	if (wstrlcpy(ret, path, slen) >= slen)
		goto error;

	if (folder &&
		(wstrlcat(ret, "/", slen) >= slen ||
		 wstrlcat(ret, folder, slen) >= slen))
			goto error;

	if (ext &&
		(wstrlcat(ret, "/", slen) >= slen ||
		 wstrlcat(ret, ext, slen) >= slen))
			goto error;

	if (wstrlcat(ret, "/", slen) >= slen ||
	    wstrlcat(ret, resource, slen) >= slen)
		goto error;

	if (access(ret, F_OK) != 0)
		goto error;

	return ret;

error:
	if (ret)
		wfree(ret);
	return NULL;
}

char *WMPathForResourceOfType(const char *resource, const char *ext)
{
	char *path, *tmp, *appdir;
	int i;
	size_t slen;

	path = tmp = appdir = NULL;

	/*
	 * Paths are searched in this order:
	 * - resourcePath/ext
	 * - dirname(argv[0])/ext
	 * - GNUSTEP_USER_ROOT/Applications/ApplicationName.app/ext
	 * - ~/.workspace/Applications/ApplicationName.app/ext
	 * - GNUSTEP_LOCAL_ROOT/Applications/ApplicationName.app/ext
	 * - /usr/local/.workspace/Applications/ApplicationName.app/ext
	 * - GNUSTEP_SYSTEM_ROOT/Applications/ApplicationName.app/ext
	 * - /usr/.workspace/Applications/ApplicationName.app/ext
	 */

	if (WMApplication.resourcePath) {
		path = checkFile(WMApplication.resourcePath, NULL, ext, resource);
		if (path)
			goto out;
	}

	if (WMApplication.argv[0]) {
		tmp = wstrdup(WMApplication.argv[0]);
		i = strlen(tmp);
		while (i > 0 && tmp[i] != '/')
			i--;
		tmp[i] = 0;
		if (i > 0) {
			path = checkFile(tmp, NULL, ext, resource);
		} else {
			path = NULL;
		}
		goto out;
	}

	slen = strlen(WMApplication.applicationName) + sizeof("Applications/.app");
	appdir = wmalloc(slen);
	if (snprintf(appdir, slen, "Applications/%s.app", WMApplication.applicationName) >= slen)
		goto out;

	path = checkFile(getenv("GNUSTEP_USER_ROOT"), appdir, ext, resource);
	if (path)
		goto out;

	path = checkFile(wusergnusteppath(), appdir, ext, resource);
	if (path)
		goto out;

	path = checkFile(getenv("GNUSTEP_LOCAL_ROOT"), appdir, ext, resource);
	if (path)
		goto out;

	path = checkFile("/usr/local/.workspace", appdir, ext, resource);
	if (path)
		goto out;

	path = checkFile(getenv("GNUSTEP_SYSTEM_ROOT"), appdir, ext, resource);
	if (path)
		goto out;

	path = checkFile("/usr/.workspace", appdir, ext, resource); /* falls through */

out:
	if (tmp)
		wfree(tmp);
	if (appdir)
		wfree(appdir);

	return path;

}
