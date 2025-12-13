
#include "config.h"

static void Sig(void)
{
    ansi_cprint_xy(0, 1, LIGHTGREEN, "BBS " BBSvers);
    ansi_cprint_xy(0, 68, LIGHTGREEN, BBSdate);
    ansi_cprint_xy(18, 62, LIGHTBLUE, "◎ [NCTUCSIE] ◎");
}

void Help(void)
{
    char conti_esc[] = "[请按任意键继续或按 Esc 跳离]";
    char anykey[] = "[请按任意键继续]";
    int i;
    extern unsigned int mode;
    ansi_clear;

    ansi_forecolor(LIGHTGRAY);

    i = 3;
    ansi_print_xy(i++, 1, "^C            结束程式");
    ansi_print_xy(i++, 1, "Enter         直接连线");
    ansi_print_xy(i++, 1, "Space         切换至下个选单");
    ansi_print_xy(i++, 1, "1-9           直接切换至其它选单");
    ansi_print_xy(i++, 1, "a-z,A-Z       快速移动选择项");
    ansi_print_xy(i++, 1, "^,$           移动选项至第一/最後一个");
    ansi_print_xy(i++, 1, "^Z            萤幕保护");
    ansi_print_xy(i++, 1, "^T            切换显示彩色");
    ansi_print_xy(i++, 1, "/             重绘萤幕");
    ansi_print_xy(i++, 1, "?             求助");
    ansi_print_xy(i++, 1, "^V            版本资讯");
    if (!DEF(DEF_SECURITY)) {
	ansi_print_xy(i++, 1, "^E            编辑自己的资料档");
	ansi_print_xy(i++, 1, "^P            以电子邮件张贴文章至此 BBS (Mailpost) [BBS]");
	ansi_print_xy(i++, 1, "^U            寄 E-Mail 给此 BBS 站内的使用者       [BBS]");
    }
    if (DEF(DEF_MAILSERV)) {
	ansi_print_xy(i++, 1, "^A            寄信给本程式作者");
	ansi_print_xy(i++, 1, "^B            索取原始程式");
/*	ansi_print_xy(i++, 1, "^F            查询资料档格式"); */
    }
    ansi_print_xy(3, 31, "其中 ^ 表 Ctrl-");

    Sig();

    if (DEF(DEF_SECURITY)) {
	ansi_cprint_xy(LINES - 1, 0, LIGHTMAGENTA, anykey);
	ansi_flush();
	getch();
	return;
    }
    ansi_cprint_xy(LINES - 1, 0, LIGHTMAGENTA, conti_esc);

    ansi_flush();
    if (getch() == 27)
	return;

    ansi_clear;

    ansi_cprint_xy(4, 1, WHITE, "本程式会依序搜寻所用的资料档如下");
    ansi_cprint_xy(6, 5, LIGHTCYAN, "-  $HOME/." CFGFILE);
    ansi_cprint_xy(7, 5, LIGHTCYAN, "-  " DEF_SYS_BBSDATA);

#if defined(BBSDATA)
    ansi_cprint_xy(8, 5, LIGHTCYAN, "-  " BBSDATA);
#endif

    ansi_cprint_xy(10, 1, WHITE, "您可以按 Ctrl-E 编辑自订资料档内容");
/*
    if(DEF(DEF_MAILSERV))
	ansi_cprint_xy(11, 1, WHITE, "关於此资料档的格式, 你可以直接按 Ctrl-F 取得参考档案");
*/
    Sig();
    ansi_cprint_xy(LINES - 1, 0, LIGHTMAGENTA, conti_esc);

    ansi_flush();
    if (getch() == 27)
	return;

    ansi_clear;

    ansi_cprint_xy(3, 2, WHITE, "命令列参数:");

    ansi_forecolor(YELLOW);
    ansi_print_xy(5, 7, "bbs 2    直接跳至第二个选单");
    ansi_print_xy(6, 7, "bbs 1a   直接连往 1a 选项");
    ansi_print_xy(7, 7, "bbs -z   直接进入萤幕保护模式");
    ansi_print_xy(8, 7, "bbs -d   印出除错资讯");
    ansi_print_xy(9, 7, "bbs -h   选项说明");

    ansi_cprint_xy( 9, 52, LIGHTGRAY, "Written By Shen Cheng-Da");
    ansi_cprint_xy(10, 52, LIGHTGRAY, "cdsheen@csie.nctu.edu.tw ");

    if (DEF(DEF_MAILSERV)) {
/*	ansi_print_xy(10, 7, "bbs -f   查询资料档格式"); */
	ansi_print_xy(11, 7, "bbs -b   取得原始程式");
    }
    Sig();

    ansi_cprint_xy(LINES - 1, 0, LIGHTMAGENTA, anykey);
    ansi_flush();
    getch();
}
