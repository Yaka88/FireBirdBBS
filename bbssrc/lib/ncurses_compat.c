/* Minimal ncurses compatibility layer for bbsnet */
/* Provides missing symbols that aren't in the minimal libncurses.a */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* Global variables that ncurses programs expect */
int LINES = 24;
int COLS = 80;

/* Minimal WINDOW structure */
typedef struct {
    int _cury, _curx;
    int _maxy, _maxx;
} WINDOW;

WINDOW _stdscr = {0, 0, 24, 80};
WINDOW *stdscr = &_stdscr;

/* Terminal state */
static struct termios orig_termios;
static int term_initialized = 0;

/* Initialize terminal to get actual dimensions */
static void init_term_size(void) {
    char *lines_env = getenv("LINES");
    char *cols_env = getenv("COLUMNS");
    
    if (lines_env) LINES = atoi(lines_env);
    if (cols_env) COLS = atoi(cols_env);
    
    /* Try to get from ioctl if available */
#ifdef TIOCGWINSZ
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1) {
        if (ws.ws_row > 0) LINES = ws.ws_row;
        if (ws.ws_col > 0) COLS = ws.ws_col;
    }
#endif
    
    _stdscr._maxy = LINES;
    _stdscr._maxx = COLS;
}

/* Window functions */
int wgetch(WINDOW *win) {
    int c;
    if (!term_initialized) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        term_initialized = 1;
    }
    c = getchar();
    return c;
}

int wrefresh(WINDOW *win) {
    fflush(stdout);
    return 0;
}

int wnoutrefresh(WINDOW *win) {
    return 0;
}

int doupdate(void) {
    fflush(stdout);
    return 0;
}

int wclear(WINDOW *win) {
    printf("\033[2J\033[H");
    fflush(stdout);
    return 0;
}

int raw(void) {
    if (!term_initialized) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        term_initialized = 1;
    }
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    return 0;
}

/* Initialization */
void __attribute__((constructor)) ncurses_compat_init(void) {
    init_term_size();
}

void __attribute__((destructor)) ncurses_compat_cleanup(void) {
    if (term_initialized) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }
}
