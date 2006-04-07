/* .bdf => .h */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	FILE *in = stdin;
	char buf[128];
	int f, g;
	int c = 0,w = 0, i, mask;
	int fw, fh, fox, foy, width, height, x, y, wcount;
	int maxy = -999;
	int ofs = 0;
	int ofstbl[256];
	char fontdesc[256];
	
	for (i = 0; i < 256; i++) {
		ofstbl[i] = 0;
	}

        if (argc < 2)
		return 0;

	strcpy(fontdesc, argv[1]);

	printf("\n");
	printf("static signed char font%sdat[]={\n\n", fontdesc);

	printf("/* data for each char is ox oy w h dwidth, then data */\n\n");

	while (fgets(buf, sizeof(buf), in) != NULL) {
		if (strncmp(buf, "FONTBOUNDINGBOX ", 16) == 0)
			sscanf(buf + 16, "%d %d %d %d", &fw, &fh, &fox, &foy);
		else if (strncmp(buf, "ENCODING ", 9) == 0)
			c = atoi(buf + 9);
		else if (strncmp(buf, "DWIDTH ", 7) == 0)
			w = atoi(buf + 7);
		else if (strncmp(buf, "BBX ", 4) == 0) {
			sscanf(buf + 4, "%d %d %d %d", &width, &height, &x, &y);
		} else if (strcmp(buf, "BITMAP\n") == 0) {
			if (c < 32 || c > 255)
				continue;
			ofstbl[c] = ofs;
			printf("/* `%c' */\n", c);
			printf("%d,%d,%d,%d,%d,\n", x, y, width, height, w);
			ofs += 5;
			if (y + height - foy > maxy)
				maxy = y + height - foy;
			while (fgets(buf, sizeof(buf), in) != NULL && strcmp(buf, "ENDCHAR\n") != 0) {
				i = 0;
				wcount = width;
				for (f = 0; f < strlen(buf) - 1; f++) {
					c = toupper(buf[f]) - 48;
					if (c > 9)
						c -= 7;
					if (c < 0 || c > 15) {
						fprintf(stderr,"error in font - bad hex!\n");
						exit(1);
					} else {
						i = i * 16 + c;
						if (f & 1) {
							for (g = 0, mask = 128; g < 8 && wcount--; g++, ofs++, mask >>= 1) {
								putchar((i & mask) ? '1' : '0');
								putchar(',');
							}
							i = 0;
						}
					}
				}
				if (f & 1) {
					for (g = 0, mask = 128; g < 8 && wcount--; g++, ofs++,mask >>= 1) {
						putchar((i & mask) ? '1' : '0');
						putchar(',');
					}
				}
				putchar('\n');
			}
			putchar('\n');
		}
	}

	printf("};\n\n");
        printf("static int font%stbl[224]={", fontdesc);

	for(f = 32; f < 256; f++) {
		if ((f & 7) == 0)
			putchar('\n');
		printf("%d,", ofstbl[f]);
	}

	printf("\n};\n\n");

	printf("void font%s_init (S_FONT_T *font)\n", fontdesc);
	printf("{\n");
	printf("\tfont->fh = %d;\n", fh);
	printf("\tfont->oy = %d;\n", foy);
	printf("\tfont->yofs = %d;\n", fh - maxy);
	printf("\t\n");
	printf("\tfont->dat = font%sdat;\n", fontdesc);
	printf("\tfont->tbl = font%stbl;\n", fontdesc);
	printf("}\n");

	exit(0);
}
