
#include <stdio.h>
#include "config.h"

static char edit_name[50];
static char flag;

char *Edit(void)
{
    extern char Editor[35];

    char command[80];

    umask(077);
    ansi_cprint(LIGHTGREEN, "\n请输入欲编辑的档案名称 (按 [Enter] 编辑新档) : ");
    ansi_flush();
    fgets(edit_name, 49, stdin);
    flag = 'N';
    if (edit_name[0] == '\n') {
	sprintf(edit_name, "/tmp/.bbs.edit.%d", getpid());
	flag = 'Y';
    } else
	edit_name[strlen(edit_name) - 1] = '\0';
    /* Edit */
    sprintf(command, "%s %s", Editor, edit_name);
    exec_wait(command, 1);

    return (edit_name);
}

void Delete(void)
{
    if (flag == 'Y')
	unlink(edit_name);
}
