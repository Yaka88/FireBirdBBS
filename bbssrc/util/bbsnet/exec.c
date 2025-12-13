
#include "config.h"
#include <signal.h>

int exec_wait(char *command, int curses)
{
    int i, pid;
    char *args[20];
    char *ch;
    char separator[] = " \t\n\r";	/* 不认识 ' 和 " */

    ch = (char *) strtok(command, separator);
    for (i = 0; ch != NULL; i++) {
	args[i] = ch;
	ch = (char *) strtok(NULL, separator);
    }
    args[i] = NULL;

    pid = fork();


    if (pid < 0) {		/* 无法 fork() */
	return (1);
    } else if (pid == 0) {	/* child process */
/*		signal(SIGINT, SIG_IGN);*/
	if (curses)
	    endwin();
	execvp(args[0], args);
    } else {			/* parent process */
/*		signal(SIGINT, SIG_IGN);*/
	waitpid(pid, NULL, 0);
	signal(SIGINT, Bye);
	return (0);
    }
}
