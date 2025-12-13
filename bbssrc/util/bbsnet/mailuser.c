
#include <stdio.h>
#include "config.h"

void Mail2user(char c)
{
    extern struct BBS bbs[BBS_Max];
    extern char Mailer[35];

    char id[30];
    char command[120];
    char subject[60];
    char *edit_name;
    time_t b_time;

    b_time = time(NULL);

    umask(077);

    ansi_clear;

    ansi_cprint(LIGHTRED, "注意: 使用本功\能前请先确定 %s BBS 有支援 Internet Mail to User\n\n", bbs[c].name);

    ansi_cprint(LIGHTGREEN, "\n请输入您要寄给 %s BBS 内的那一位使用者\n", bbs[c].name);

    do {
	ansi_cprint(LIGHTCYAN, "\nMail to: ");
	ansi_flush();
	fgets(id, 29, stdin);
	id[strlen(id) - 1] = '\0';
	ansi_cprint(YELLOW, "\n确定寄给 ");
	ansi_cprint(LIGHTMAGENTA, id);
	ansi_cprint(YELLOW, " 吗? [Y] ");
	ansi_flush();
	fgets(command, 20, stdin);

    } while ((command[0] != '\n') && (toupper(command[0]) != 'Y'));

    ansi_cprint(WHITE, "\n信件的标题是: ");
    ansi_flush();
    fgets(subject, 59, stdin);
    subject[strlen(subject) - 1] = '\0';
    edit_name = Edit();

    /* Mail post */
    if (!isdigit(bbs[c].hostname[0]))
	sprintf(command, "%s -s \"%s\" %s.bbs@%s < %s", Mailer, subject, id, bbs[c].hostname, edit_name);
    else
	sprintf(command, "%s -s \"%s\" %s.bbs@[%s]. < %s", Mailer, subject, id, bbs[c].hostname, edit_name);

    system(command);

    Delete();

    sprintf(command, "%s/%s", id, subject);

    log(LOG_MAILUSER, b_time, bbs[c].hostname, command);
}
