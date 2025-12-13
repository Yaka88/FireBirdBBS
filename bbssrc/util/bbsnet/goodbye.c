
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

void Welcome(void)
{

    char buffer[80];
    int i;
    extern FILE *fp;
    extern long connect_time, idle_time, now_time;
    extern unsigned int mode;

    ansi_clear;

    ansi_cprint_xy(0, 0, WHITE, Header);
    idle_time += time(NULL) - now_time;

    if (DEF(DEF_COUNTTIME)) {
	ansi_forecolor(LIGHTBLUE);
	ansi_print_xy(0, 54, "┌——————————┐");
	ansi_print_xy(1, 54, "│发呆时间：     分钟 │");
	ansi_print_xy(2, 54, "│连线时间：     分钟 │");
	ansi_print_xy(3, 54, "└——————————┘");

	ansi_forecolor(LIGHTRED);
	ansi_print_xy(1, 67, "%3d", idle_time / 60);
	ansi_print_xy(2, 67, "%3d", connect_time / 60);
    }
    i = 1;
    fseek(fp, 0L, SEEK_SET);
    ansi_forecolor(LIGHTGREEN);
    while (fgets(buffer, Rec_Len, fp) != NULL) {
	buffer[strlen(buffer) - 1] = '\0';
	if ((buffer[0] == '*') && (!strncasecmp(&buffer[3], "Issue", 5)))
	    ansi_print_xy(++i, 0, &buffer[10]);
    }

    ansi_gotoxy(i + 2, 0);
    ansi_forecolor(LIGHTGRAY);
    ansi_flush();
    fclose(fp);

/*    log(LOG_EXITPROG, time(NULL) - idle_time - connect_time, NULL, NULL);*/

    exit(0);
}

void Bye(int ignore)
{
    alarm(0);
    clear();
    refresh();
    endwin();
    Welcome();
}
