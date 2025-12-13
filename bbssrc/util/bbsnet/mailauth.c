
#include <stdio.h>
#include "config.h"

void Mail2author(void)
{
    extern char Mailer[35];
    extern char cfg;

    char command[80];
    char subject[60];
    char *edit_name;
    time_t b_time;
    FILE *fp;

    time(&b_time);

    umask(077);

    ansi_clear;

    ansi_cprint(YELLOW, "寄信给作者..\n\n");

    ansi_cprint(WHITE, "信件的标题是: ");
    ansi_flush();

    fgets(subject, 59, stdin);
    subject[strlen(subject) - 1] = '\0';
    edit_name = Edit();

    if( ( fp = fopen( edit_name, "a") ) != NULL ) {
	print_debug( fp );
	fclose(fp);
    }

    /* Mail post */
    sprintf(command, "%s -s \"%s (BBS " BBSvers ") \" " AUTHOR " < %s",
	    Mailer, subject, edit_name);

    system(command);

    Delete();

    log(LOG_MAILAUTH, b_time, subject, NULL);
}
