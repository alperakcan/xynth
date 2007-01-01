/***************************************************************************
    begin                : Sat Nov 8 2003
    copyright            : (C) 2003 - 2007 by Alper Akcan
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
#include <string.h>
#include <ctype.h>

#define NR_KEYS	128
#define MAX_KEYNAME_LEN	20
#define PLAIN	0
#define SHIFT	1
#define ALTGR	2
#define LAST	3

int main (int argc, char *argv[])
{
	int i;
	int j;
	int k;
	FILE *mapfile;

	char codenames[NR_KEYS][LAST][MAX_KEYNAME_LEN];
	int size = sizeof(codenames);

	char buf[400];
	char buf1[400];
	char buf2[400];
	char buf3[400];
	char buf4[400];
	int int1;

	if (argc < 2) {
		printf("\n");
		printf("%s is to prepare a hader file from a map file.\n", argv[0]);
		printf("\n");
		printf("Ex:\n"
		       "\tmkdir kbdmap\n"
		       "\tfiles included in the us.map.gz are;\n"
		       "\t\tcompose.latin1\n"
		       "\t\teuro1.map\n"
		       "\t\tlinux-keys-bare.inc\n"
		       "\t\tlinux-with-alt-and-altgr.inc\n"
		       "\t\tqwerty-layout.inc\n"
		       "\tcopy all_of_them_and_the_us.map.gz to kbdmap\n"
		       "\tgunzip us.map.gz\n"
		       "\tcat * > usmap\n"
		       "\t%s usmap > usmap.h\n\n", argv[0]);
		printf("Alper Akcan, distchx@yahoo.com\n\n");
		return 0;
	}

	memset(codenames, 0, size);
	mapfile = fopen(argv[1], "rt");

	for (i = 0; i < NR_KEYS; i++) {
		for (j = 0; j < LAST; j++) {
			sprintf(codenames[i][j], "\"\"");
		}
	}

	while (!feof(mapfile)) {
		fgets(buf, 400, mapfile);
		sscanf(buf, "%s", buf1);
		buf3[0] = '\0';
		buf4[0] = '\0';

		if (strcmp(buf1, "keycode") == 0) {
			sscanf(buf, "%s %d = %s %s %s", buf1, &int1, buf2, buf3, buf4);
			sprintf(codenames[int1][PLAIN], "\"%s\"", buf2);

			if ((strcmp(buf3, "#") != 0) &&  (buf3[0] != '\0')) {
				sprintf(codenames[int1][SHIFT], "\"%s\"", buf3);

				if ((strcmp(buf4, "#") != 0) &&  (buf4[0] != '\0')) {
					sprintf(codenames[int1][ALTGR], "\"%s\"", buf4);
				}
			}
		}

		if (strcmp(buf1, "plain") == 0) {
			sscanf(buf, "%s %s %d = %s", buf1, buf1, &int1, buf2);
			sprintf(codenames[int1][PLAIN], "\"%s\"", buf2);
		}

		if (strcmp(buf1, "shift") == 0) {
			sscanf(buf, "%s %s %d = %s", buf1, buf1, &int1, buf2);
			sprintf(codenames[int1][SHIFT], "\"%s\"", buf2);
		}

		if (strcmp(buf1, "altgr") == 0) {
			sscanf(buf, "%s %s %d = %s", buf1, buf1, &int1, buf2);
			sprintf(codenames[int1][ALTGR], "\"%s\"", buf2);
		}

	}

	for (i = 0; i < NR_KEYS; i++) {
		for (j = 0; j < strlen(codenames[i][PLAIN]); j++) {
			codenames[i][PLAIN][j] = toupper(codenames[i][PLAIN][j]);
		}

		for (j = 0; j < strlen(codenames[i][SHIFT]); j++) {
			codenames[i][SHIFT][j] = toupper(codenames[i][SHIFT][j]);
		}

		for (j = 0; j < strlen(codenames[i][ALTGR]); j++) {
			codenames[i][ALTGR][j] = toupper(codenames[i][ALTGR][j]);
		}
	}

	for (j = 65; j < 91; j++) {
		sprintf(buf1, "\"%c\"", j);
		for (i = 0; i < NR_KEYS; i++) {
			if (strcmp(codenames[i][PLAIN], buf1) == 0) {
				sprintf(codenames[i][PLAIN], "\"%c\"", j + 32);
				sprintf(codenames[i][SHIFT], "\"%c\"", j);
			}
		}
	}
	for (i = 0; i < NR_KEYS; i++) {
		if (strcmp(codenames[i][PLAIN], "\"KP_0\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"INSERT\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_1\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"END\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_2\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"DOWN\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_3\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"PAGEDOWN\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_4\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"LEFT\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_5\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_6\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"RIGHT\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_7\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"HOME\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_8\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"UP\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_9\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"PAGEUP\"");
		}
		if (strcmp(codenames[i][PLAIN], "\"KP_PERIOD\"") == 0) {
			sprintf(codenames[i][SHIFT], "\"REMOVE\"");
		}
	}
	
	printf("#define KEYCODE_KEYS\t%d\n", NR_KEYS);
	printf("#define KEYCODE_MAXNAMELEN\t%d\n", MAX_KEYNAME_LEN);
	printf("#define KEYCODE_PLAIN\t%d\n", PLAIN);
	printf("#define KEYCODE_SHIFT\t%d\n", SHIFT);
	printf("#define KEYCODE_ALTGR\t%d\n", ALTGR);
	printf("#define KEYCODE_FLAGS\t%d\n", LAST);
	printf("\n");

	printf("typedef enum {\n");
	k = 0;
	for (j = 0; j < 3; j++) {
	    switch (j) {
		case PLAIN:	printf("/* Plain */\n");	break;
		case SHIFT:	printf("/* Shift */\n");	break;
		case ALTGR:	printf("/* ALTGR */\n");	break;
	    }
	    for (i = 0; i < NR_KEYS; i++) {
		char *name;
		name = (char *) calloc(sizeof(char), strlen(codenames[i][j]));
		sprintf(name, "%s", codenames[i][j] + 1);
		name[strlen(name) - 1] = '\0';
		if (strlen(name) > 0) {
		    printf("/* %3d */  XYNTHK_%s,\n", k++, name);
		}
		free(name);
	    }
	}
	printf("/* %3d */  XYNTHK_CODES\n", k);
	printf("} XYNTHK_CODE;\n");
	
	printf("\n");
	
	printf("static XYNTHK_CODE kbd_keycodes_[KEYCODE_KEYS][KEYCODE_FLAGS][KEYCODE_MACNAMELEN] = {\n");
	printf("/*%10s %18s %18s %17s*/\n", "keycode,", "plain name,", "shift name,", "altgr name ");
    
        for (i = 0; i < NR_KEYS; i++) {
	    printf("/* %3d */  {", i);
	    
	    for (j = 0; j < 3; j++) {
	        char *name;
		name = (char *) calloc(sizeof(char), strlen(codenames[i][j]));
		sprintf(name, "%s", codenames[i][j] + 1);
		name[strlen(name) - 1] = '\0';
		if (strlen(name) > 0) {
		    printf("XYNTHK_%s", name);
		} else {
		    printf("XYNTH_NOCODE");
		}
		if (j < 2) {
		    printf(",\t");
		}
		free(name);
	    }
	    printf("},\n");
	}
	printf("};\n");
	
	printf("\n");	

	printf("static char kbd_keycodes[KEYCODE_KEYS][KEYCODE_FLAGS][KEYCODE_MAXNAMELEN] = {\n");
	printf("/*%10s %18s %18s %17s*/\n", "keycode,", "plain name,", "shift name,", "altgr name ");

	for (i = 0; i < NR_KEYS; i++) {
		printf("/* %3d */  {%18s,%18s,%18s},\n", i, codenames[i][PLAIN],
							    codenames[i][SHIFT],
							    codenames[i][ALTGR]);
	}

	printf("};\n");
	
	return 0;
}
