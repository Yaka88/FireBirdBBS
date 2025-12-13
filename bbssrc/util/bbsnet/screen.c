
#include "config.h"
#include <stdarg.h>

extern unsigned int mode;

char _ANSI_BUF[ANSI_LINEMAX];
char ANSI_BUF[ANSI_BUF_MAX];
int ANSI_IDX = 0;

unsigned char ANSI_DEF_FORE = LIGHTGRAY;
unsigned char ANSI_NOW_FORE;

char *ansi_fore_color[16] = {
    "\033[30;0m", "\033[34;0m", "\033[32;0m",
    "\033[36;0m", "\033[31;0m", "\033[35;0m",
    "\033[33;0m", "\033[37;0m",
    "\033[30;1m", "\033[34;1m", "\033[32;1m",
    "\033[36;1m", "\033[31;1m", "\033[35;1m",
    "\033[33;1m", "\033[37;1m"
};

char *ansi_back_color[8] = {
    "\033[40m", "\033[44m", "\033[42m",
    "\033[46m", "\033[41m", "\033[45m",
    "\033[43m", "\033[47m"
};

char nn[] = "";

char *ansi_fore_mono[16] = {
    "\033[0m", "\033[0m", "\033[0m",
    "\033[0m", "\033[0m", "\033[0m",
    "\033[0m", "\033[0m",
    "\033[1m", "\033[1m", "\033[1m",
    "\033[1m", "\033[1m", "\033[1m",
    "\033[1m", "\033[1m"
};

char *ansi_back_mono[8] = {nn, nn, nn, nn, nn, nn, nn, nn};

char **ansi_fore = ansi_fore_color;
char **ansi_back = ansi_back_color;

void ansi_show(void)
{
    if (DEF(DEF_SHOWANSI)) {
	ansi_fore = ansi_fore_color;
	ansi_back = ansi_back_color;
    } else {
	ansi_fore = ansi_fore_mono;
	ansi_back = ansi_back_mono;
    }
}

void ansi_bufs(char *buf)
{
    int i, len;
    len = strlen(buf);
    for (i = 0; i < len; i++) {
	ANSI_BUF[ANSI_IDX++] = buf[i];
	if (ANSI_IDX == ANSI_BUF_MAX)
	    ansi_flush();
    }
}

void ansi_flush(void)
{
    if (ANSI_IDX != 0) {
	write(1, ANSI_BUF, ANSI_IDX);
	ANSI_IDX = 0;
    }
}


void ansi_gotoxy(int x, int y)
{
    sprintf(_ANSI_BUF, "\033[%d;%dH", x + 1, y + 1);
    ansi_bufs(_ANSI_BUF);
}

void ansi_forecolor(unsigned char fore)
{
	ANSI_NOW_FORE = ANSI_DEF_FORE = fore;
    ansi_bufs(ansi_fore[fore]);
}

void ansi_backcolor(unsigned char back)
{
    ansi_bufs(ansi_back[back]);
}

void ansi_color(unsigned char fore, unsigned char back)
{
    ansi_bufs(ansi_fore[ANSI_NOW_FORE = ANSI_DEF_FORE = fore]);
    ansi_bufs(ansi_back[back]);
}

void ansi_normal(void)
{
    ansi_bufs(ansi_fore_color[ANSI_NOW_FORE = ANSI_DEF_FORE = LIGHTGRAY]);
    ansi_bufs(ansi_back_color[BLACK]);
}

void ansi_print(char *format,...)
{
    va_list args;
    if (ANSI_NOW_FORE != ANSI_DEF_FORE)
	ansi_bufs(ansi_fore[ANSI_NOW_FORE = ANSI_DEF_FORE]);
    va_start(args, format);
    vsprintf(_ANSI_BUF, format, args);
    ansi_bufs(_ANSI_BUF);
    va_end(args);
}

void ansi_cprint(unsigned char fore, char *format,...)
{
    va_list args;

    ansi_bufs(ansi_fore[ANSI_NOW_FORE = fore]);

    va_start(args, format);
    vsprintf(_ANSI_BUF, format, args);
    ansi_bufs(_ANSI_BUF);
    va_end(args);
}

void ansi_print_xy(int x, int y, char *format,...)
{
    va_list args;
    if (ANSI_NOW_FORE != ANSI_DEF_FORE)
	ansi_bufs(ansi_fore[ANSI_NOW_FORE = ANSI_DEF_FORE]);
    ansi_gotoxy(x, y);

    va_start(args, format);
    vsprintf(_ANSI_BUF, format, args);
    ansi_bufs(_ANSI_BUF);
    va_end(args);
}

void ansi_cprint_xy(int x, int y, unsigned char fore, char *format,...)
{
    va_list args;

    ansi_gotoxy(x, y);
    ansi_bufs(ansi_fore[ANSI_NOW_FORE = fore]);

    va_start(args, format);
    vsprintf(_ANSI_BUF, format, args);
    ansi_bufs(_ANSI_BUF);
    va_end(args);
}

void ansi_ccprint_xy(int x, int y, unsigned char fore, unsigned char back, char *format,...)
{
    va_list args;

    ansi_gotoxy(x, y);

    ansi_bufs(ansi_fore[ANSI_NOW_FORE = fore]);
    ansi_bufs(ansi_back[back]);

    va_start(args, format);
    vsprintf(_ANSI_BUF, format, args);
    ansi_bufs(_ANSI_BUF);
    va_end(args);
}

void HelpBar(unsigned char color, char *str)
{
    ansi_cprint_xy(LINES, 0, color, "%-68s", str);
}
