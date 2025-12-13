
#include <ncurses.h>

int main(void)
{

    initscr();
    cbreak();
    nonl();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    if (has_colors() != TRUE) {
	endwin();
	printf("No Color Support!\n");
	exit(1);
    }
    start_color();

    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    attrset(COLOR_PAIR(1));
    mvprintw(5, 10, "Haha");

    attrset(COLOR_PAIR(2));
    mvprintw(6, 10, "Haha");

    attrset(COLOR_PAIR(3));
    mvprintw(7, 10, "Haha");

    refresh();
    getch();
    endwin();
}
