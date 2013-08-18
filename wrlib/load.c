/* load.c - load image from file
 *
 * Raster graphics library
 *
 * Copyright (c) 1997-2003 Alfredo K. Kojima
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 */

#include <config.h>

#include <errno.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#ifdef USE_PNG
#include <png.h>
#endif

#include "wraster.h"
#include "imgformat.h"

#define	RETRY( x )	do {				\
				x;			\
			} while (errno == EINTR);

typedef struct RCachedImage {
	RImage *image;
	char *file;
	time_t last_modif;	/* last time file was modified */
	time_t last_use;	/* last time image was used */
} RCachedImage;

/*
 * Size of image cache
 */
static int RImageCacheSize = -1;

/*
 * Max. size of image to store in cache
 */
static int RImageCacheMaxImage = -1;	/* 0 = any size */

#define IMAGE_CACHE_SIZE	8

#define IMAGE_CACHE_MAX_IMAGE	64*64

static RCachedImage *RImageCache;


static WRImgFormat identFile(const char *path);


char **RSupportedFileFormats(void)
{
	static char *tmp[IM_TYPES + 1];
	int i = 0;

	/* built-in */
	tmp[i++] = "XPM";
	/* built-in */
	tmp[i++] = "PPM";
#ifdef USE_TIFF
	tmp[i++] = "TIFF";
#endif
#ifdef USE_PNG
	tmp[i++] = "PNG";
#endif
#ifdef USE_JPEG
	tmp[i++] = "JPEG";
#endif
#ifdef USE_GIF
	tmp[i++] = "GIF";
#endif
	tmp[i] = NULL;

	return tmp;
}

static void init_cache(void)
{
	char *tmp;

	tmp = getenv("RIMAGE_CACHE");
	if (!tmp || sscanf(tmp, "%i", &RImageCacheSize) != 1) {
		RImageCacheSize = IMAGE_CACHE_SIZE;
	}
	if (RImageCacheSize < 0)
		RImageCacheSize = 0;

	tmp = getenv("RIMAGE_CACHE_SIZE");
	if (!tmp || sscanf(tmp, "%i", &RImageCacheMaxImage) != 1) {
		RImageCacheMaxImage = IMAGE_CACHE_MAX_IMAGE;
	}

	if (RImageCacheSize > 0) {
		RImageCache = malloc(sizeof(RCachedImage) * RImageCacheSize);
		if (RImageCache == NULL) {
			printf("wrlib: out of memory for image cache\n");
			return;
		}
		memset(RImageCache, 0, sizeof(RCachedImage) * RImageCacheSize);
	}
}

RImage *RLoadImage(RContext * context, const char *file, int index)
{
	RImage *image = NULL;
	int i;
	struct stat st;

	assert(file != NULL);

	if (RImageCacheSize < 0) {
		init_cache();
	}

	if (RImageCacheSize > 0) {

		for (i = 0; i < RImageCacheSize; i++) {
			if (RImageCache[i].file && strcmp(file, RImageCache[i].file) == 0) {

				if (stat(file, &st) == 0 && st.st_mtime == RImageCache[i].last_modif) {
					RImageCache[i].last_use = time(NULL);

					return RCloneImage(RImageCache[i].image);

				} else {
					free(RImageCache[i].file);
					RImageCache[i].file = NULL;
					RReleaseImage(RImageCache[i].image);
				}
			}
		}
	}

	switch (identFile(file)) {
	case IM_ERROR:
		return NULL;

	case IM_UNKNOWN:
		RErrorCode = RERR_BADFORMAT;
		return NULL;

	case IM_XPM:
		image = RLoadXPM(context, file);
		break;

#ifdef USE_TIFF
	case IM_TIFF:
		image = RLoadTIFF(file, index);
		break;
#endif				/* USE_TIFF */

#ifdef USE_PNG
	case IM_PNG:
		image = RLoadPNG(context, file);
		break;
#endif				/* USE_PNG */

#ifdef USE_JPEG
	case IM_JPEG:
		image = RLoadJPEG(context, file);
		break;
#endif				/* USE_JPEG */

#ifdef USE_GIF
	case IM_GIF:
		image = RLoadGIF(file, index);
		break;
#endif				/* USE_GIF */

	case IM_PPM:
		image = RLoadPPM(file);
		break;

	default:
		RErrorCode = RERR_BADFORMAT;
		return NULL;
	}

	/* store image in cache */
	if (RImageCacheSize > 0 && image &&
	    (RImageCacheMaxImage == 0 || RImageCacheMaxImage >= image->width * image->height)) {
		time_t oldest = time(NULL);
		int oldest_idx = 0;
		int done = 0;

		for (i = 0; i < RImageCacheSize; i++) {
			if (!RImageCache[i].file) {
				RImageCache[i].file = malloc(strlen(file) + 1);
				strcpy(RImageCache[i].file, file);
				RImageCache[i].image = RCloneImage(image);
				RImageCache[i].last_modif = st.st_mtime;
				RImageCache[i].last_use = time(NULL);
				done = 1;
				break;
			} else {
				if (oldest > RImageCache[i].last_use) {
					oldest = RImageCache[i].last_use;
					oldest_idx = i;
				}
			}
		}

		/* if no slot available, dump least recently used one */
		if (!done) {
			free(RImageCache[oldest_idx].file);
			RReleaseImage(RImageCache[oldest_idx].image);
			RImageCache[oldest_idx].file = malloc(strlen(file) + 1);
			strcpy(RImageCache[oldest_idx].file, file);
			RImageCache[oldest_idx].image = RCloneImage(image);
			RImageCache[oldest_idx].last_modif = st.st_mtime;
			RImageCache[oldest_idx].last_use = time(NULL);
		}
	}

	return image;
}

char *RGetImageFileFormat(const char *file)
{
	switch (identFile(file)) {
	case IM_XPM:
		return "XPM";

#ifdef USE_TIFF
	case IM_TIFF:
		return "TIFF";
#endif				/* USE_TIFF */

#ifdef USE_PNG
	case IM_PNG:
		return "PNG";
#endif				/* USE_PNG */

#ifdef USE_JPEG
	case IM_JPEG:
		return "JPEG";
#endif				/* USE_JPEG */

#ifdef USE_GIF
	case IM_GIF:
		return "GIF";
#endif				/* USE_GIF */

	case IM_PPM:
		return "PPM";

	default:
		return NULL;
	}
}

static WRImgFormat identFile(const char *path)
{
	FILE *file;
	unsigned char buffer[32];
	size_t nread;

	assert(path != NULL);

	RETRY( file = fopen(path, "rb") )
	if (file == NULL) {
		RErrorCode = RERR_OPEN;
		return IM_ERROR;
	}

	RETRY( nread = fread(buffer, 1, sizeof(buffer), file) )
	if (nread < sizeof(buffer) || ferror(file)) {
		RETRY( fclose(file) )
		RErrorCode = RERR_READ;
		return IM_ERROR;
	}
	RETRY( fclose(file) )

	/* check for XPM */
	if (strncmp((char *)buffer, "/* XPM */", 9) == 0)
		return IM_XPM;

	/* check for TIFF */
	if ((buffer[0] == 'I' && buffer[1] == 'I' && buffer[2] == '*' && buffer[3] == 0)
	    || (buffer[0] == 'M' && buffer[1] == 'M' && buffer[2] == 0 && buffer[3] == '*'))
		return IM_TIFF;

#ifdef USE_PNG
	/* check for PNG */
	if (!png_sig_cmp(buffer, 0, 8))
		return IM_PNG;
#endif

	/* check for raw PPM or PGM */
	if (buffer[0] == 'P' && (buffer[1] == '5' || buffer[1] == '6'))
		return IM_PPM;

	/* check for JPEG */
	if (buffer[0] == 0xff && buffer[1] == 0xd8)
		return IM_JPEG;

	/* check for GIF */
	if (buffer[0] == 'G' && buffer[1] == 'I' && buffer[2] == 'F')
		return IM_GIF;

	return IM_UNKNOWN;
}
