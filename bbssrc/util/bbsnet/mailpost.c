
#include <stdio.h>
#include "config.h"

void MailPost(char c)
{

    extern struct BBS bbs[BBS_Max];
    extern char Mailer[35];

    FILE *pfp, *mfp;
    char buffer[80];
    char subject[71];
    char board[20];

    char *edit_name, *passwd;
    char mail_name[30];
    time_t b_time;

    b_time = time(NULL);

    ansi_clear;
    edit_name = Edit();

    sprintf(mail_name, "/tmp/.mail.post.%d", getpid());

    if ((mfp = fopen(mail_name, "w")) == NULL) {
	printf("无法开启档案 %s. 程式中断..\n", mail_name);
	return;
    }
    ansi_clear;

    ansi_cprint(LIGHTRED, " 注意: 使用 Mail post 功\能前请先确定 %s BBS 有支援 Mail Post 的功\能\n\n", bbs[c].name);

    ansi_cprint(LIGHTMAGENTA, "请输入您在 %s BBS 的帐号\n\nUsername: ", bbs[c].name);

    ansi_flush();

    fgets(buffer, 70, stdin);
    buffer[strlen(buffer) - 1] = '\0';
    fprintf(mfp, "#name: %s\n", buffer);

    passwd = NULL;

    do {
	if (passwd != NULL) {
	    ansi_cprint(LIGHTRED, "\n两次输入的密码不同, 请重新输入");
	    ansi_flush();
	}
	passwd = (char *) getpass("\nPassword: ");
	strcpy(buffer, passwd);
	passwd = (char *) getpass("Password again: ");

    } while (strcmp(passwd, buffer));

    fprintf(mfp, "#password: %s\n", buffer);

    ansi_cprint(YELLOW, "\n\n请输入所欲 Post 的讨论区\n\nBoard: ");
    ansi_flush();
    fgets(board, 19, stdin);
    board[strlen(board) - 1] = '\0';
    fprintf(mfp, "#board: %s\n", board);

    ansi_cprint(LIGHTCYAN, "\n请输入文章之标题\n\nSubject: ");
    ansi_flush();

    fgets(subject, 70, stdin);
    subject[strlen(subject) - 1] = '\0';
    fprintf(mfp, "#title: %s\n\n", subject);

    if ((pfp = fopen(edit_name, "r")) == NULL) {
	printf("无法开启档案 %s. 程式中断..\n", edit_name);
	fclose(mfp);
	return;
    }
    while (fgets(buffer, 90, pfp) != NULL)
	fputs(buffer, mfp);

    fclose(mfp);
    fclose(pfp);

    /* Mail post */
    if (!isdigit(bbs[c].hostname[0]))
	sprintf(buffer, "%s bbs@%s < %s", Mailer, bbs[c].hostname, mail_name);
    else
	sprintf(buffer, "%s bbs@[%s]. < %s", Mailer, bbs[c].hostname, mail_name);
    system(buffer);
    unlink(mail_name);
    Delete();

    sprintf(buffer, "%s/%s", board, subject);
    log(LOG_MAILPOST, b_time, bbs[c].hostname, buffer);

}
