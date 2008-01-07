/***************************************************************************
    begin                : Mon Dec 22 2007
    copyright            : (C) 2007 - 2008 by Alper Akcan
    email                : distchx@yahoo.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <xynth.h>

#define DEBUG   0
#define DEFINED 0

void print_usage_exit (void)
{
	exit(-1);
}

#if DEFINED
s_rect_t rects_defined[] = {
/*   5 */ {   2,   4,   1,   2 },
/*   9 */ {   0,   0,   4,   5 },
/*  10 */ {   0,   0,   3,   5 },
};
#endif

int find_in_rects (s_rect_t *rects, int nrects, s_rect_t *rect)
{
	int n;
	for (n = 0; n < nrects; n++) {
		if (rects[n].x == rect->x &&
		    rects[n].y == rect->y &&
		    rects[n].w == rect->w &&
		    rects[n].h == rect->h) {
			return 0;
		}
	}
	return -1;
}

s_rect_t * create_rects (int width, int height, int nrects)
{
	int n;
	s_rect_t *r;
	s_rect_t *rs;
	rs = (s_rect_t *) malloc(nrects * sizeof(s_rect_t));
	if (rs == NULL) {
		return NULL;
	}
	n = 0;
	r = rs;
#if DEFINED
	memcpy(rs, rects_defined, sizeof(rects_defined));
#else
#if DEBUG
	printf("s_rect_t rects_defined[] = {\n");
#endif
	while (n < nrects) {
		r->x = rand() % width;
		r->y = rand() % height;
		r->w = rand() % (width - r->x);
		r->h = rand() % (height - r->y);
		if (r->w <= 0 || r->h <= 0) {
			continue;
		}
#if DEBUG
		printf("/* %3d */ { %3d, %3d, %3d, %3d },\n", n, r->x, r->y, r->w, r->h);
#endif
		if (find_in_rects(rs, n, r) != 0) {
			n++;
			r++;
		}
	}
#if DEBUG
	printf("};\n");
#endif
#endif
	return rs;
}

char * create_matrix (s_region_t *region, int width, int height, int order)
{
	int n;
	int x;
	int y;
	s_rect_t *r;
	char *matrix;
	s_rect_t extents;
	n = s_region_num_rectangles(region);
	r = s_region_rectangles(region);
	s_region_extents(region, &extents);
	matrix = (char *) malloc(sizeof(char) * width * height);
	if (matrix == NULL) {
		return NULL;
	}
	memset(matrix, 0, sizeof(char) * width * height);
	for (y = 0; y < region->extents.h; y++) {
		for (x = 0; x < region->extents.w; x++) {
			*(matrix + ((region->extents.y + y) * width) + (region->extents.x + x)) = !order;
		}
	}
	for (n = 0; n < s_region_num_rectangles(region); n++) {
		for (y = 0; y < r->h; y++) {
			for (x = 0; x < r->w; x++) {
				*(matrix + ((r->y + y) * width) + (r->x + x)) = order;
			}
		}
		r++;
	}
#if DEBUG
	{
		char *m;
		m = matrix;
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				if (*m) {
					printf("*");
				} else {
					printf(".");
				}
				m++;
			}
			printf("\n");
		}
	}
#endif
	return matrix;
}

int test_addrect (int width, int height, int nrects)
{
	int n;
	s_rect_t *rects;
	s_region_t *region;
	unsigned long long times[2];
	printf("running %s\n", __FUNCTION__);
	rects = create_rects(width, height, nrects);
	if (rects == NULL) {
		return -1;
	}
	times[0] = s_gettimeofday();
	if (s_region_create(&region)) {
		return -2;
	}
	for (n = 0; n < nrects; n++) {
		if (s_region_addrect(region, &rects[n])) {
			return -3;
		}
	}
	times[1] = s_gettimeofday();
	if (nrects != s_region_num_rectangles(region)) {
		return -4;
	}
	for (n = 0; n < s_region_num_rectangles(region); n++) {
		if (find_in_rects(rects, nrects, &region->rects[n])) {
			return -5;
		}
	}
	for (n = 0; n < nrects; n++) {
		if (find_in_rects(region->rects, nrects, &rects[n])) {
			return -6;
		}
	}
	if (s_region_destroy(region)) {
		return -7;
	}
	free(rects);
	printf("took %llu ms\n", times[1] - times[0]);
	return 0;
}

int test_delrect (int width, int height, int nrects)
{
	int n;
	int d;
	s_rect_t *rects;
	s_region_t *region;
	unsigned long long times[3];
	printf("running %s\n", __FUNCTION__);
	rects = create_rects(width, height, nrects);
	if (rects == NULL) {
		return -1;
	}
	if (s_region_create(&region)) {
		return -2;
	}
	for (n = 0; n < nrects; n++) {
		if (s_region_addrect(region, &rects[n])) {
			return -3;
		}
	}
	times[2] = 0;
	while (s_region_num_rectangles(region) != 0) {
		d = rand() % nrects;
		times[0] = s_gettimeofday();
		if (s_region_delrect(region, &rects[d])) {
			return -4;
		}
		times[1] = s_gettimeofday();
		times[2] += times[1] - times[0];
		for (n = 0; n < s_region_num_rectangles(region); n++) {
			if (find_in_rects(rects, nrects, &region->rects[n])) {
				return -5;
			}
		}
		if (find_in_rects(region->rects, s_region_num_rectangles(region), &rects[d]) == 0) {
			return -6;
		}
	}
	if (s_region_destroy(region)) {
		return -7;
	}
	free(rects);
	printf("took %llu ms\n", times[2]);
	return 0;
}

int test_copy (int width, int height, int nrects)
{
	int n;
	s_rect_t *rects;
	s_rect_t copye;
	s_region_t *copy;
	s_rect_t regione;
	s_region_t *region;
	unsigned long long times[2];
	printf("running %s\n", __FUNCTION__);
	rects = create_rects(width, height, nrects);
	if (rects == NULL) {
		return -1;
	}
	if (s_region_create(&region)) {
		return -2;
	}
	for (n = 0; n < nrects; n++) {
		if (s_region_addrect(region, &rects[n])) {
			return -3;
		}
	}
	times[0] = s_gettimeofday();
	if (s_region_copy(&copy, region)) {
		return -4;
	}
	times[1] = s_gettimeofday();
	if (s_region_num_rectangles(region) != s_region_num_rectangles(copy)) {
		return -5;
	}
	if (s_region_extents(region, &regione)) {
		return -6;
	}
	if (s_region_extents(copy, &copye)) {
		return -7;
	}
	if (regione.x != copye.x ||
	    regione.y != copye.y ||
	    regione.w != copye.w ||
	    regione.h != copye.h) {
		return -8;
	}
	for (n = 0; n < s_region_num_rectangles(copy); n++) {
		if (find_in_rects(rects, nrects, &copy->rects[n])) {
			return -9;
		}
	}
	if (s_region_destroy(copy)) {
		return -10;
	}
	if (s_region_destroy(region)) {
		return -11;
	}
	free(rects);
	printf("took %llu ms\n", times[1] - times[0]);
	return 0;
}

int test_extents (int width, int height, int nrects)
{
	printf("running %s\n", __FUNCTION__);
	return 0;
}

int test_unify (int width, int height, int nrects)
{
	int n;
	char *regionm;
	char *regionu;
	s_rect_t *rects;
	s_rect_t extentsr;
	s_rect_t extentsu;
	s_region_t *region;
	unsigned long long times[6];
	printf("running %s\n", __FUNCTION__);
	rects = create_rects(width, height, nrects);
	if (rects == NULL) {
		return -1;
	}
	if (s_region_create(&region)) {
		return -2;
	}
	for (n = 0; n < nrects; n++) {
		if (s_region_addrect(region, &rects[n])) {
			return -3;
		}
	}
	if (s_region_extents(region, &extentsr)) {
		return -4;
	}
	times[0] = s_gettimeofday();
	regionm = create_matrix(region, width, height, 1);
	times[1] = s_gettimeofday();
	if (regionm == NULL) {
		return -5;
	}
	printf("region nrects: %d\n", region->nrects);
	times[2] = s_gettimeofday();
	if (s_region_unify(region)) {
		return -6;
	}
	times[3] = s_gettimeofday();
	printf("region nrects: %d\n", region->nrects);
	if (s_region_extents(region, &extentsu)) {
		return -7;
	}
	if (extentsr.x != extentsu.x ||
	    extentsr.y != extentsu.y ||
	    extentsr.w != extentsu.w ||
	    extentsr.h != extentsu.h) {
		return -8;
	}
	times[4] = s_gettimeofday();
	regionu = create_matrix(region, width, height, 1);
	times[5] = s_gettimeofday();
	if (regionu == NULL) {
		return -9;
	}
	if (s_region_destroy(region)) {
		return -10;
	}
	if (memcmp(regionu, regionm, sizeof(char) * width * height)) {
		return -11;
	}
	free(rects);
	free(regionm);
	free(regionu);
	printf("took %llu ms (created in %llu ms, %llu ms)\n", times[3] - times[2], times[1] - times[0], times[5] - times[4]);
	return 0;
}

int test_reverse (int width, int height, int nrects)
{
	int n;
	char *regionm;
	char *regionu;
	s_rect_t *rects;
	s_rect_t extentsr;
	s_rect_t extentsu;
	s_region_t *region;
	s_region_t *regionr;
	unsigned long long times[6];
	printf("running %s\n", __FUNCTION__);
	rects = create_rects(width, height, nrects);
	if (rects == NULL) {
		return -1;
	}
	if (s_region_create(&region)) {
		return -2;
	}
	for (n = 0; n < nrects; n++) {
		if (s_region_addrect(region, &rects[n])) {
			return -3;
		}
	}
	if (s_region_extents(region, &extentsr)) {
		return -4;
	}
	times[0] = s_gettimeofday();
	regionm = create_matrix(region, width, height, 0);
	times[1] = s_gettimeofday();
	if (regionm == NULL) {
		return -5;
	}
	printf("region nrects: %d\n", region->nrects);
	times[2] = s_gettimeofday();
	if (s_region_reverse(region, &regionr)) {
		return -6;
	}
	times[3] = s_gettimeofday();
	printf("region nrects: %d\n", regionr->nrects);
	if (s_region_extents(regionr, &extentsu)) {
		return -7;
	}
	times[4] = s_gettimeofday();
	regionu = create_matrix(regionr, width, height, 1);
	times[5] = s_gettimeofday();
	if (regionu == NULL) {
		return -8;
	}
	if (s_region_destroy(region)) {
		return -9;
	}
	if (s_region_destroy(regionr)) {
		return -10;
	}
	if (memcmp(regionu, regionm, sizeof(char) * width * height)) {
		return -11;
	}
	free(rects);
	free(regionm);
	free(regionu);
	printf("took %llu ms (created in %llu ms, %llu ms)\n", times[3] - times[2], times[1] - times[0], times[5] - times[4]);
	return 0;
}

int test_combine (int width, int height, int nrects)
{
	int n;
	char *regionm;
	char *regionu;
	s_rect_t *rects;
	s_rect_t extents;
	s_region_t *region;
	unsigned long long times[2];
	printf("running %s\n", __FUNCTION__);
	rects = create_rects(width, height, nrects);
	if (rects == NULL) {
		return -1;
	}
	if (s_region_create(&region)) {
		return -2;
	}
	for (n = 0; n < nrects; n++) {
		if (s_region_addrect(region, &rects[n])) {
			return -3;
		}
	}
	if (s_region_extents(region, &extents)) {
		return -4;
	}
	regionm = create_matrix(region, width, height, 1);
	if (regionm == NULL) {
		return -5;
	}
	times[0] = s_gettimeofday();
	if (s_region_combine(region)) {
		return -6;
	}
	times[1] = s_gettimeofday();
	regionu = create_matrix(region, width, height, 1);
	if (regionu == NULL) {
		return -7;
	}
	if (s_region_destroy(region)) {
		return -8;
	}
	if (memcmp(regionu, regionm, sizeof(char) * width * height)) {
		return -9;
	}
	free(rects);
	free(regionm);
	free(regionu);
	printf("took %llu ms\n", times[1] - times[0]);
	return 0;
}

int (*test_functions[]) (int, int, int) = {
	&test_addrect,
	&test_delrect,
	&test_copy,
	&test_extents,
	&test_unify,
	&test_reverse,
	&test_combine,
	NULL
};

int main (int argc, char *argv[])
{
	int c;
	int r;
	int width = 0;
	int height = 0;
	int nrects = 0;
	unsigned int seed = 0;
	int (**tf) (int, int, int);
	
	while ((c = getopt(argc, argv, "w:h:n:s:?")) != -1) {
		switch (c) {
			case 'w':
				width = atoi(optarg);
				break;
			case 'h':
				height = atoi(optarg);
				break;
			case 'n':
				nrects = atoi(optarg);
				break;
			case 's':
				seed = (unsigned int) atoi(optarg);
				break;
			case '?':
			default:
				print_usage_exit();
				break;
		}
	}

	if (width == 0 ||
	    height == 0 ||
	    nrects == 0) {
		print_usage_exit();
	}
	
	if (seed == 0) {
		seed = s_gettimeofday();
	}
	
	printf("width = %d, height = %d, nrects = %d, seed = %u\n", width, height, nrects, seed);
	
	srand(seed);
	
	for (tf = test_functions; *tf; tf++) {
		r = (*tf)(width, height, nrects);
		if (r != 0) {
			printf("test failed, exiting with %d\n", r);
			exit(r);
		}
	}
	return 0;
}
