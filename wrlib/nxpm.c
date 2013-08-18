/* nxpm.c - load "normalized" XPM image
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "wraster.h"
#include "imgformat.h"

/*
 * Restricted support for XPM images.
 *
 * The images must be in the following "normalized" format:
 *
 *
 * line   	content
 * 1	        signature comment
 * 2		ignored ( normally "static char *xpm[] = {" )
 * 3		"width height color_count chars" where chars is 1 or 2
 * 4	        color definitions. Only c values with #rrggbb or #rrrrggggbbb
 *			format OR None
 * n		data
 *
 * - no comments or blank lines are allowed, except for the signature
 * - all lines must have at most 256 characters
 * - no white spaces allowed at left of each line
 */

#define LINEWIDTH 64

#ifndef USE_XPM

static void free_color_symbol_table(unsigned char *color_table[],
				     unsigned short *symbol_table)
{
	if (color_table[0])
		free(color_table[0]);
	if (color_table[1])
		free(color_table[1]);
	if (color_table[2])
		free(color_table[2]);
	if (color_table[3])
		free(color_table[3]);
	if (symbol_table)
		free(symbol_table);
}

RImage *RGetImageFromXPMData(RContext * context, char **data)
{
	RImage *image = NULL;
	unsigned char *color_table[4] = { NULL, NULL, NULL, NULL };
	unsigned short *symbol_table = NULL;
	unsigned char *r, *g, *b, *a;
	int i, j, k, line = 0;
	int transp;
	unsigned short color;
	int bsize;
	int w, h, ccount, csize;

	if (sscanf(data[line++], "%i %i %i %i", &w, &h, &ccount, &csize) != 4
	    || w <= 0 || h <= 0 || ccount <= 0 || csize <= 0)
		goto bad_format;

	if (csize != 1 && csize != 2)
		goto bad_format;

	color_table[0] = malloc(ccount);
	color_table[1] = malloc(ccount);
	color_table[2] = malloc(ccount);
	color_table[3] = malloc(ccount);
	symbol_table = malloc(ccount * sizeof(unsigned short));

	bsize = csize * w + 16;

	if (!color_table[0] || !color_table[1] || !color_table[2] || !color_table[3] || !symbol_table || !bsize) {
		RErrorCode = RERR_NOMEMORY;
		free_color_symbol_table(color_table, symbol_table);
		return NULL;
	}

	transp = 0;
	/* get color table */
	for (i = 0; i < ccount; i++) {
		symbol_table[i] = data[line][0];
		if (csize == 2)
			symbol_table[i] |= data[line][1] << 8;

		j = csize;
		while (data[line][j] != '#' && data[line][j] != 0 && data[line][j] != 'N')
			j++;

		if (data[line][j] == '#') {
			unsigned int red, green, blue;

			k = 0;
			j++;
			while (data[line][j + k] != 0)
				k++;
			if (k == 6) {
				if (sscanf(&(data[line][j]), "%2x%2x%2x", &red, &green, &blue) != 3)
					goto bad_format;
			} else if (k == 12) {
				if (sscanf(&(data[line][j]), "%4x%4x%4x", &red, &green, &blue) != 3)
					goto bad_format;
				red >>= 8;
				green >>= 8;
				blue >>= 8;
			} else
				goto bad_format;

			color_table[0][i] = red;
			color_table[1][i] = green;
			color_table[2][i] = blue;
			color_table[3][i] = 255;
		} else if (strncmp(&(data[line][j]), "None", 4) == 0 || strncmp(&(data[line][j]), "none", 4) == 0) {
			color_table[3][i] = 0;
			transp = 1;
		} else {
			goto bad_format;
		}
		line++;
	}

	image = RCreateImage(w, h, transp);
	if (!image) {
		free_color_symbol_table(color_table, symbol_table);
		return NULL;
	}

	r = image->data;
	g = image->data + 1;
	b = image->data + 2;
	if (image->format == RRGBAFormat)
		a = image->data + 3;
	else
		a = NULL;

	for (i = 0; i < h; i++) {
		if (csize == 1) {
			for (j = 0; j < w; j++) {
				color = data[line][j];

				for (k = 0; k < ccount; k++) {
					if (symbol_table[k] == color)
						break;
				}
				if (k == ccount)
					k = 0;

				*r = color_table[0][k];
				*g = color_table[1][k];
				*b = color_table[2][k];
				if (a) {
					*a = color_table[3][k];
					r += 4;
					g += 4;
					b += 4;
					a += 4;
				} else {
					r += 3;
					g += 3;
					b += 3;
				}
			}
		} else {
			for (j = 0; j < w * 2; j++) {
				color = data[line][j++];
				color |= data[line][j];

				for (k = 0; k < ccount; k++) {
					if (symbol_table[k] == color)
						break;
				}
				if (k == ccount)
					k = 0;

				*r = color_table[0][k];
				*g = color_table[1][k];
				*b = color_table[2][k];
				if (a) {
					*a = color_table[3][k];
					r += 4;
					g += 4;
					b += 4;
					a += 4;
				} else {
					r += 3;
					g += 3;
					b += 3;
				}
			}
		}
		line++;
	}

	free_color_symbol_table(color_table, symbol_table);
	return image;

 bad_format:
	RErrorCode = RERR_BADIMAGEFILE;
	free_color_symbol_table(color_table, symbol_table);
	if (image)
		RReleaseImage(image);
	return NULL;
}

RImage *RLoadXPM(RContext * context, const char *file)
{
	RImage *image = NULL;
	char line[LINEWIDTH + 1];
	char *buffer = NULL;
	unsigned char *color_table[4] = { NULL, NULL, NULL, NULL };
	unsigned short *symbol_table = NULL;
	unsigned char *r, *g, *b, *a;
	int i, j, k;
	int transp;
	unsigned short color;
	int bsize;
	int w, h, ccount, csize;
	FILE *f;

	f = fopen(file, "rb");
	if (!f) {
		RErrorCode = RERR_OPEN;
		return NULL;
	}
	/* sig */
	if (!fgets(line, LINEWIDTH, f))
		goto bad_file;
	/* declaration */
	if (!fgets(line, LINEWIDTH, f))
		goto bad_file;

	/* data */
	if (!fgets(line, LINEWIDTH, f))
		goto bad_file;

	if (line[0] == '/')
		if (!fgets(line, LINEWIDTH, f))
			goto bad_file;

	if (sscanf(line, "\"%i %i %i %i\"", &w, &h, &ccount, &csize) != 4
	    || w <= 0 || h <= 0 || ccount <= 0 || csize <= 0)
		goto bad_file;

	if (csize != 1 && csize != 2)
		goto bad_format;

	color_table[0] = malloc(ccount);
	color_table[1] = malloc(ccount);
	color_table[2] = malloc(ccount);
	color_table[3] = malloc(ccount);
	symbol_table = malloc(ccount * sizeof(unsigned short));

	bsize = csize * w + 16;
	buffer = malloc(bsize);

	if (!color_table[0] || !color_table[1] || !color_table[2] ||
	    !color_table[3] || !symbol_table || !bsize || !buffer) {
		RErrorCode = RERR_NOMEMORY;
		fclose(f);
		free_color_symbol_table(color_table, symbol_table);
		if (buffer)
			free(buffer);
		return NULL;
	}

	transp = 0;
	/* get color table */
	for (i = 0; i < ccount; i++) {
		if (!fgets(line, LINEWIDTH, f))
			goto bad_file;
		if (line[0] == '/')
			if (!fgets(line, LINEWIDTH, f))
				goto bad_file;

		symbol_table[i] = line[1];
		if (csize == 2)
			symbol_table[i] |= line[2] << 8;

		j = csize + 1;
		while (line[j] != '#' && line[j] != '"' && line[j] != 0 && line[j] != 'N')
			j++;

		if (line[j] == '#') {
			unsigned int red, green, blue;

			k = 0;
			j++;
			while (line[j + k] != '"' && line[j + k] != 0)
				k++;
			if (k == 6) {
				if (sscanf(&(line[j]), "%2x%2x%2x", &red, &green, &blue) != 3)
					goto bad_format;
			} else if (k == 12) {
				if (sscanf(&(line[j]), "%4x%4x%4x", &red, &green, &blue) != 3)
					goto bad_format;
				red >>= 8;
				green >>= 8;
				blue >>= 8;
			} else
				goto bad_format;

			color_table[0][i] = red;
			color_table[1][i] = green;
			color_table[2][i] = blue;
			color_table[3][i] = 255;
		} else if (strncmp(&(line[j]), "None", 4) == 0 || strncmp(&(line[j]), "none", 4) == 0) {
			color_table[3][i] = 0;
			transp = 1;
		} else {
			goto bad_format;
		}
	}

	image = RCreateImage(w, h, transp);
	if (!image) {
		fclose(f);
		free_color_symbol_table(color_table, symbol_table);
		if (buffer)
			free(buffer);
		return NULL;
	}

	r = image->data;
	g = image->data + 1;
	b = image->data + 2;
	if (image->format == RRGBAFormat)
		a = image->data + 3;
	else
		a = NULL;

	for (i = 0; i < h; i++) {
		if (!fgets(buffer, bsize, f))
			goto bad_file;
		if (buffer[0] == '/')
			if (!fgets(buffer, bsize, f))
				goto bad_file;

		if (csize == 1) {
			for (j = 1; j <= w; j++) {
				color = buffer[j];

				for (k = 0; k < ccount; k++) {
					if (symbol_table[k] == color)
						break;
				}
				if (k == ccount)
					k = 0;

				*r = color_table[0][k];
				*g = color_table[1][k];
				*b = color_table[2][k];
				if (a) {
					*a = color_table[3][k];
					r += 4;
					g += 4;
					b += 4;
					a += 4;
				} else {
					r += 3;
					g += 3;
					b += 3;
				}
			}
		} else {
			for (j = 1; j <= w * 2; j++) {
				color = buffer[j++];
				color |= buffer[j] << 8;

				for (k = 0; k < ccount; k++) {
					if (symbol_table[k] == color)
						break;
				}
				if (k == ccount) {
					k = 0;
				}

				*r = color_table[0][k];
				*g = color_table[1][k];
				*b = color_table[2][k];
				if (a) {
					*a = color_table[3][k];
					r += 4;
					g += 4;
					b += 4;
					a += 4;
				} else {
					r += 3;
					g += 3;
					b += 3;
				}
			}
		}
	}

	fclose(f);
	free_color_symbol_table(color_table, symbol_table);
	if (buffer)
		free(buffer);
	return image;

 bad_format:
	RErrorCode = RERR_BADIMAGEFILE;
	fclose(f);
	free_color_symbol_table(color_table, symbol_table);
	if (buffer)
		free(buffer);
	if (image)
		RReleaseImage(image);
	return NULL;

 bad_file:
	RErrorCode = RERR_BADIMAGEFILE;
	fclose(f);
	free_color_symbol_table(color_table, symbol_table);
	if (buffer)
		free(buffer);
	if (image)
		RReleaseImage(image);
	return NULL;
}

#endif

typedef struct XPMColor {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	int index;
	struct XPMColor *next;
} XPMColor;

#define I2CHAR(i)	((i)<12 ? (i)+'0' : ((i)<38 ? (i)+'A'-12 : (i)+'a'-38))
#define CINDEX(xpmc)	(((unsigned)(xpmc)->red)<<16|((unsigned)(xpmc)->green)<<8|((unsigned)(xpmc)->blue))

static XPMColor *lookfor(XPMColor * list, int index)
{
	if (!list)
		return NULL;

	for (; list != NULL; list = list->next) {
		if (CINDEX(list) == index)
			return list;
	}
	return NULL;
}

/*
 * Looks for the color in the colormap and inserts if it is not found.
 *
 * list is a binary search list. The unbalancing problem is just ignored.
 *
 * Returns False on error
 */
static Bool addcolor(XPMColor ** list, unsigned r, unsigned g, unsigned b, int *colors)
{
	XPMColor *tmpc;
	XPMColor *newc;
	int index;

	index = r << 16 | g << 8 | b;
	tmpc = *list;

	tmpc = lookfor(*list, index);

	if (tmpc)
		return True;

	newc = malloc(sizeof(XPMColor));

	if (!newc) {

		RErrorCode = RERR_NOMEMORY;

		return False;
	}

	newc->red = r;
	newc->green = g;
	newc->blue = b;
	newc->next = *list;
	*list = newc;

	(*colors)++;

	return True;
}

static char *index2str(char *buffer, int index, int charsPerPixel)
{
	int i;

	for (i = 0; i < charsPerPixel; i++) {
		buffer[i] = I2CHAR(index & 63);
		index >>= 6;
	}
	buffer[i] = 0;

	return buffer;
}

static void outputcolormap(FILE * file, XPMColor * colormap, int charsPerPixel)
{
	int index;
	char buf[128];

	if (!colormap)
		return;

	for (index = 0; colormap != NULL; colormap = colormap->next, index++) {
		colormap->index = index;
		fprintf(file, "\"%s c #%02x%02x%02x\",\n",
			index2str(buf, index, charsPerPixel), colormap->red, colormap->green, colormap->blue);
	}
}

static void freecolormap(XPMColor * colormap)
{
	XPMColor *tmp;

	while (colormap) {
		tmp = colormap->next;
		free(colormap);
		colormap = tmp;
	}
}

/* save routine is common to internal support and library support */
Bool RSaveXPM(RImage * image, const char *filename)
{
	FILE *file;
	int x, y;
	int colorCount = 0;
	int charsPerPixel;
	XPMColor *colormap = NULL;
	XPMColor *tmpc;
	int i;
	int ok = 0;
	unsigned char *r, *g, *b, *a;
	char transp[16];
	char buf[128];

	file = fopen(filename, "wb+");
	if (!file) {
		RErrorCode = RERR_OPEN;
		return False;
	}

	fprintf(file, "/* XPM */\n");

	fprintf(file, "static char *image[] = {\n");

	r = image->data;
	g = image->data + 1;
	b = image->data + 2;
	if (image->format == RRGBAFormat)
		a = image->data + 3;
	else
		a = NULL;

	/* first pass: make colormap for the image */
	if (a)
		colorCount = 1;
	for (y = 0; y < image->height; y++) {
		for (x = 0; x < image->width; x++) {
			if (!a || *a > 127) {
				if (!addcolor(&colormap, *r, *g, *b, &colorCount)) {
					goto uhoh;
				}
			}
			if (a) {
				r += 4;
				g += 4;
				b += 4;
				a += 4;
			} else {
				r += 3;
				g += 3;
				b += 3;
			}
		}
	}

	charsPerPixel = 1;
	while ((1 << charsPerPixel * 6) < colorCount)
		charsPerPixel++;

	/* write header info */
	fprintf(file, "\"%i %i %i %i\",\n", image->width, image->height, colorCount, charsPerPixel);

	/* write colormap data */
	if (a) {
		for (i = 0; i < charsPerPixel; i++)
			transp[i] = ' ';
		transp[i] = 0;

		fprintf(file, "\"%s c None\",\n", transp);
	}

	i = 0;
	outputcolormap(file, colormap, charsPerPixel);

	r = image->data;
	g = image->data + 1;
	b = image->data + 2;
	if (image->format == RRGBAFormat)
		a = image->data + 3;
	else
		a = NULL;

	/* write data */
	for (y = 0; y < image->height; y++) {

		fprintf(file, "\"");

		for (x = 0; x < image->width; x++) {

			if (!a || *a > 127) {
				tmpc = lookfor(colormap, (unsigned)*r << 16 | (unsigned)*g << 8 | (unsigned)*b);

				fprintf(file, "%s", index2str(buf, tmpc->index, charsPerPixel));
			} else {
				fprintf(file, "%s", transp);
			}

			if (a) {
				r += 4;
				g += 4;
				b += 4;
				a += 4;
			} else {
				r += 3;
				g += 3;
				b += 3;
			}
		}

		if (y < image->height - 1)
			fprintf(file, "\",\n");
		else
			fprintf(file, "\"};\n");
	}

	ok = 1;
 uhoh:
	errno = 0;
	fclose(file);
	if (ok && errno == ENOSPC) {
		RErrorCode = RERR_WRITE;
	}

	freecolormap(colormap);

	return ok ? True : False;
}
