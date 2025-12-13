
#include "config.h"

void drawbox1(int x1, int y1, int x2, int y2)
{
    int i;
    char buf[80];

    ansi_print_xy(y1, x1, "¡õ");
    ansi_print_xy(y1, x2 - 1, "¡õ");
    ansi_print_xy(y2, x1, "¡õ");
    ansi_print_xy(y2, x2 - 1, "¡õ");

    buf[0] = '\0';
    for (i = x1 + 2; i < x2 - 1; i += 2)
	strcat(buf, "¡ª");

    ansi_print_xy(y1, x1 + 2, buf);
    ansi_print_xy(y2, x1 + 2, buf);

    for (i = y1 + 1; i < y2; i++) {
	ansi_print_xy(i, x1, "©¦");
	ansi_print_xy(i, x2 - 1, "©¦");
    }

}
void drawbox2(int x1, int y1, int x2, int y2)
{
    int i;
    char buf[80];

    ansi_print_xy(y1, x1, "¡õ");
    ansi_print_xy(y1, x2 - 1, "¡õ");
    ansi_print_xy(y2, x1, "¡õ");
    ansi_print_xy(y2, x2 - 1, "¡õ");

    buf[0] = '\0';
    for (i = x1 + 2; i < x2 - 1; i += 2)
	strcat(buf, "¡õ");

    ansi_print_xy(y1, x1 + 2, buf);
    ansi_print_xy(y2, x1 + 2, buf);

    for (i = y1 + 1; i < y2; i++) {
	ansi_print_xy(i, x1, "¡õ");
	ansi_print_xy(i, x2 - 1, "¡õ");
    }

}
