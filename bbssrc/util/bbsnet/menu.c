
#include <stdio.h>
#include "config.h"

extern char Title[10][20];

extern struct BBS bbs[BBS_Max];
extern unsigned char x[BBS_Max];
extern unsigned char y[BBS_Max];
extern char label[3];
extern char symbol[62];
extern int H, n;

void menu(void)
{

    int i;

    H = LINES - 7;

    for (i = 0; i < n; i++) {
	x[i] = 2 + i % H;
	y[i] = 4 + 20 * ((int) (i / H));
	label[1] = symbol[i];
	ansi_cprint_xy(x[i], y[i] - 1, LIGHTGREEN, label);
	ansi_cprint_xy(x[i], y[i] + 2, LIGHTGRAY, bbs[i].name);
    }
    ansi_flush();
}

void mainmenu(int s, int a)
{
    int i;
    int y;

    y = 6;
    ansi_forecolor(LIGHTGRAY);
    for (i = 1; i <= a; i++) {
	if (i == s) {
	    ansi_ccprint_xy(y++, 62, LIGHTGREEN, BLUE, "[%1d] %-9s", i, Title[i]);
	    ansi_backcolor(BLACK);
	} else
	    ansi_print_xy(y++, 62, " %1d. %-9s", i, Title[i]);
    }
    drawbox1(60, 5, COLS - 3, y);
}
