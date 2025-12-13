
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>

#include "config.h"

unsigned int mode;
struct BBS bbs[BBS_Max];
int hosttype;
int H, n;
unsigned char x[BBS_Max];
unsigned char y[BBS_Max];

char Username[15];
char mailbox[35];
/*
 int mailbox_size;
 struct stat mailbox_status;
*/
char Editor[35];
char Mailer[35];
char Gopher[35];
char Ftp[35];
char Telnet[40];
char cport[6];
char label[5];
char sel_label[5];
char symbol[54];
char Title[MAIN_MENU][20];
char command[120];
FILE *fp;
long now;
long now_time;
long idle_time;
long connect_time;
long last_key_time;
long offscreen;
char cfg;
char logfile[80];

static void Clock(int);

static int info(int c)
{

    if (hosttype == Host_GOPHER) {
	if (Gopher[0])
	    strcpy(command, Gopher);
	else {
	    strcpy(command, "[错误] 找不到 Gopher client !");
	    return (0);
	}
    } else if (hosttype == Host_FTP) {
	if (Ftp[0])
	    strcpy(command, Ftp);
	else {
	    strcpy(command, "[错误] 找不到 FTP client !");
	    return (0);
	}
    } else
	strcpy(command, Telnet);

    strcat(command, " ");
    strcat(command, bbs[c].hostname);

    sprintf(cport, "%-4d", bbs[c].port);

    if (bbs[c].port != 23) {
	strcat(command, " ");
	strcat(command, cport);
    }
    return (1);
}


static void Init(void)
{
    doupdate();
    Clock(0);
}

static void leave(void)
{
    ansi_clear;
    ansi_flush();
    alarm(0);
    wnoutrefresh(stdscr);
    endwin();
}

int s, secs;

char helpbar1[] = "[Ctrl-C] 结束  [Enter] 连线  [Space] 其他目录  [?] 求助";
char helpbar2[] = "[?] 求助  [/] 重绘萤幕  [!] 萤幕保护  [1-9] 其它目录";
char helpbar3[] = "[Enter] 连线   BBS- [Tab] Mailpost  [Ctrl-U] 寄信";
char *helpbar[] = {helpbar1, helpbar2, helpbar3};
#define     HELPNUM 3

int showbar = HELPNUM - 1;

char divide_line[] = "——————————————————————————————————————";

static void redraw(void)
{

    ansi_clear;
    ansi_forecolor(LIGHTGRAY);
    drawbox2(0, 1, COLS - 1, LINES - 2);
    ansi_print_xy(LINES - 5, 2, divide_line);
    mainmenu(s, secs);
    ansi_cprint_xy(0, 0, WHITE, "  ◎ %s ◎\0", Title[s]);

    Clock(0);
    menu();
    ansi_flush();
}

void print_debug(FILE *fp)
{
	fprintf( fp, "\n     version: " BBSvers " ("BBSdate")\n");
	fprintf( fp, "   user name: %s\n", Username);
	fprintf( fp, "     mailbox: %s\n", mailbox);
	fprintf( fp, "      editor: %s\n", Editor);
	fprintf( fp, "      mailer: %s\n", Mailer);
	fprintf( fp, "      gopher: %s\n", Gopher);
	fprintf( fp, "         ftp: %s\n", Ftp);
	fprintf( fp, "      telnet: %s\n", Telnet);
	fprintf( fp, "   data file: %c\n\n", cfg);
}

static int Ask(char *msg)
{
    char filename[25];
    FILE *fp;

    sprintf(filename, "/tmp/.bbs.ask.%d", getpid());

    if ((fp = fopen(filename, "w")) == NULL)
	return 0;

    print_debug(fp);

    fclose(fp);
    sprintf(command, "%s -s \"[BBS] Request %s\" " AUTHOR " < %s", Mailer, msg, filename);
    system(command);
    unlink(filename);
    return (1);
}

static int chk_newmail()
{
	struct stat buf;
	if (mailbox != (char *) 0 && stat (mailbox, &buf) >= 0 &&
		buf.st_atime < buf.st_mtime && buf.st_size > 0) {
		return 1;
	}
	return 0;
}

static void Saver(int ignore)
{

/*    struct stat now_status; */

    long int now;
    struct tm *tms;
    static int show;

    show = 0;
    nodelay(stdscr, TRUE);
    while (getch() == ERR) {
	time(&now);
	tms = localtime(&now);

	if (tms->tm_sec == 0) {
	    ansi_clear;
	    if (tms->tm_min == 0)
		ansi_bufs("\007");

/*	    if ((show != 1) && (stat(mailbox, &now_status) >= 0)
		    && (mailbox_size < now_status.st_size))		*/

	    show = chk_newmail();

	    if (DEF(DEF_MAILCHECK) && (show == 1) &&
		    (!(tms->tm_min % MAILCHKBEEP)))
		ansi_bufs("\007");
	}
	ansi_cprint_xy((tms->tm_min + 1) % (LINES - 1), tms->tm_sec,
		 LIGHTGREEN, "  [%.4d/%02d/%02d %.2d:%.2d]", tms->tm_year+1900,
		  tms->tm_mon + 1, tms->tm_mday, tms->tm_hour, tms->tm_min);

	if (DEF(DEF_MAILCHECK) && show )
	    ansi_cprint_xy((tms->tm_min) % (LINES - 1), tms->tm_sec, LIGHTRED,
		     "  [%.2d:%.2d] 您有新信件", tms->tm_hour, tms->tm_min);

	ansi_flush();
	sleep(1);
    }
    nodelay(stdscr, FALSE);
    time(&last_key_time);
}


static void Clock(int ignore)
{
/*    struct stat now_status; */

    long int now;
    struct tm *tms;

    time(&now);


    tms = localtime(&now);

/*    if (DEF(DEF_MAILCHECK) && (stat(mailbox, &now_status) >= 0)
	    && (mailbox_size < now_status.st_size))			*/

    if( DEF(DEF_MAILCHECK) && chk_newmail() )
	ansi_cprint_xy(LINES - 1, 69, LIGHTRED, "\033[5m\007您有新信件\033[0m");

    ansi_cprint_xy(0, COLS - 17, YELLOW, "%04d/%02d/%02d  %.2d:%.2d",
    tms->tm_year+1900, tms->tm_mon + 1, tms->tm_mday, tms->tm_hour, tms->tm_min);

    if ((tms->tm_min == 0) && (tms->tm_sec < 15))
	ansi_cprint_xy(LINES - 1, 69, LIGHTRED, "\033[5m\007  整点报时\033[0m");
    else if (tms->tm_min == 1)
	ansi_cprint_xy(LINES - 1, 69, LIGHTRED, "          ");

    HelpBar(LIGHTCYAN, helpbar[showbar = (++showbar) % HELPNUM]);
    ansi_flush();

    signal(SIGALRM, Clock);
    alarm(10);

}


int main(int argc, char *argv[])
{

    int c;
    int i, j, key, callkey;
    char buffer[Rec_Len + 1];
    long loc[MAIN_MENU + 1];
    struct stat file_status;
    char usercfg[50];

    struct passwd *pws;
    char *ptr, *ptr2;

    now_time = time(NULL);
    idle_time = connect_time = 0;

    strcpy(label, " ?.");
    strcpy(sel_label, "[?]");
    strcpy(symbol, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    if ((pws = getpwuid(getuid())) == NULL) {
	strcpy(usercfg, (char *) getenv("HOME"));
	strcpy(Username, (char *) getenv("USER"));
    } else {
	strcpy(usercfg, pws->pw_dir);
	strcpy(Username, pws->pw_name);
    }


    sprintf(mailbox, DEFAULT_MAILBOX "/%s", Username);
/*
    if (stat(mailbox, &mailbox_status) >= 0)
	mailbox_size = mailbox_status.st_size;
    else {
	sprintf(mailbox, MAILBOX2 "%s", Username);

	if (stat(mailbox, &mailbox_status) >= 0)
	   mailbox_size = mailbox_status.st_size;
	else
	    mailbox_size = 0;

    }
*/
    if ((ptr = (char *) getenv("EDITOR")) != NULL)
	strcpy(Editor, ptr);
    else
	Editor[0] = '\0';

    Gopher[0] = '\0';

    strcat(usercfg, "/.");
    strcat(usercfg, CFGFILE);

    cfg = 'u';
    if ((fp = fopen(usercfg, "r")) == NULL) {
	cfg = 's';
	if ((fp = fopen(DEF_SYS_BBSDATA, "r")) == NULL) {
#if defined(BBSDATA)
	    cfg = 'd';
	    fp = fopen(BBSDATA, "r");
#else
	    cfg = 'n';
	    ;
#endif
	}
    }

    ansi_show();

    if (fp == NULL) {

	ansi_forecolor(LIGHTGRAY);
	ansi_clear;
	ansi_print(Header "\n\n");
	ansi_print(" 网路资源程式资料档不存在，程式中止执行!\n");
	ansi_print(" 请设法在你的系统安装一个资料档，本程式搜寻资料档的顺序为:\n\n");
	ansi_print("   -  ~/." CFGFILE "\n");
	ansi_print("   -  /usr/local/etc/" CFGFILE "\n");
#if defined(BBSDATA)
	ansi_print("   -  " BBSDATA "\n");
#endif
	ansi_print("\n");
	ansi_flush();

	exit(2);
    }
    i = secs = 0;
    j = 1;
    Gopher[0] = Ftp[0] = '\0';

    mode = DEF_MAILCHECK | DEF_COUNTTIME | DEF_MAILSERV | DEF_SHOWANSI;

    offscreen = 90;

    while (fgets(buffer, Rec_Len, fp) != NULL) {
	switch (buffer[0]) {
	  case '*':
	    buffer[strlen(buffer) - 1] = '\0';

	    ptr = (char *) strchr(buffer, '#');
	    if (ptr) {
		*ptr-- = '\0';
		while (isspace(*ptr))
		    *ptr-- = '\0';
	    }
	    ptr = buffer;
	    for (; *ptr != '\0' && *ptr != '['; ptr++);
	    ptr++;

	    for (ptr2 = ptr; *ptr2 != '\0' && *ptr2 != ']'; ptr2++);
	    if (*ptr2)
		ptr2++;
	    while ((*ptr2 != '\0') && (*ptr2 == ' '))
		ptr2++;

	    if ((!strncasecmp(ptr, "editor", 6))
		    && (Editor[0] == '\0'))
		strcpy(Editor, ptr2);
	    else if (!strncasecmp(ptr, "telnet", 6))
		strcpy(Telnet, ptr2);
	    else if (!strncasecmp(ptr, "mailer", 6))
		strcpy(Mailer, ptr2);
	    else if (!strncasecmp(ptr, "gopher", 6))
		strcpy(Gopher, ptr2);
	    else if (!strncasecmp(ptr, "ftp", 3))
		strcpy(Ftp, ptr2);
	    else if (!strncasecmp(ptr, "title", 5))
		strcpy(Title[j - 1], ptr2);
	    else if (!strncasecmp(ptr, "section", 7))
		loc[j++] = loc[0];
	    else if (!strncasecmp(ptr, "mailcheck", 9))
		SETMODE(DEF_MAILCHECK, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "counttime", 9))
		SETMODE(DEF_COUNTTIME, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "mailserv", 8))
		SETMODE(DEF_MAILSERV, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "security", 8))
		SETMODE(DEF_SECURITY, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "etenmode", 8))
		SETMODE(DEF_ETENMODE, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "showansi", 8))
		SETMODE(DEF_SHOWANSI, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "logging", 7))
		SETMODE(DEF_LOGGING, toupper(*ptr2) == 'T');
	    else if (!strncasecmp(ptr, "logfile", 7))
		strcpy(logfile, ptr2);
	    else if (!strncasecmp(ptr, "offscreen", 9))
		offscreen = atol(ptr2);
	    break;
	}
	loc[0] = ftell(fp);
    }

    secs = j - 1;

    ansi_show();

    if (DEF(DEF_SECURITY))
	mode &= ~DEF_MAILSERV;

    for (ptr = Editor, j = 0; *ptr != ' ' && *ptr != '\0'; ptr++, j++)
	command[j] = *ptr;
    command[j] = '\0';

    if ((command[0] == '/') && (stat(command, &file_status) < 0)) {
	printf("检查所使用的 editor : %s\n"
	       "错误: 找不到 editor - 请更改设定档中 [editor] 一栏\n"
	       "      或者修改你的 EDITOR 环境变数\n", Editor);
	exit(1);
    }
    for (ptr = Mailer, j = 0; *ptr != ' ' && *ptr != '\0'; ptr++, j++)
	command[j] = *ptr;
    command[j] = '\0';

    if (stat(command, &file_status) < 0) {
	printf("错误: 找不到 `mail' - 请更改设定档中 [mailer] 一栏\n");
	exit(1);
    }
    for (ptr = Telnet, j = 0; *ptr != ' ' && *ptr != '\0'; ptr++, j++)
	command[j] = *ptr;
    command[j] = '\0';

    if (stat(command, &file_status) < 0) {
	printf("错误: 找不到 `telnet' - 请更改设定档中 [telnet] 一栏\n");
	exit(1);
    }
    for (ptr = Gopher, j = 0; *ptr != ' ' && *ptr != '\0'; ptr++, j++)
	command[j] = *ptr;
    command[j] = '\0';

    if ((command[0] != '\0') && (stat(command, &file_status) < 0)) {
	Gopher[0] = '\0';
    }
    for (ptr = Ftp, j = 0; *ptr != ' ' && *ptr != '\0'; ptr++, j++)
	command[j] = *ptr;
    command[j] = '\0';

    if ((command[0] != '\0') && (stat(command, &file_status) < 0)) {
	Ftp[0] = '\0';
    }
    callkey = 0;
    s = 1;
    if (argc > 1) {

	if (argv[1][0] == '-') {
	    if (DEF(DEF_MAILSERV) && argv[1][1] == 'f') {
		Ask("Format");
		printf("\n一天内您将可收到资料档格式样本\n\n");
	    } else if (DEF(DEF_MAILSERV) && argv[1][1] == 's') {
		Ask("Source");
		printf("\n一天内您将可收到原始程式\n\n");
	    } else if (argv[1][1] == 'z') {
		callkey = 1;
		key = '!';
	    } else if (argv[1][1] == 'd') {
		print_debug(stderr);
	    } else {
		printf(Header "\n\n");
		if (argv[1][1] != 'h')
		    printf("[错误] 程式不认识的参数:  `%s'\n\n", argv[1]);
		if (DEF(DEF_MAILSERV)) {
		    printf("Usage: bbs [-s] [-h] [-z] [-d] [1-9[a-Z]]\n\n");
/*		    printf("            -f  取得资料档格式\n");*/
		    printf("            -s  取得原始程式码\n");
		} else
		    printf("Usage: bbs [-h] [-z] [1-9[a-Z]]\n\n");

		printf("            -h  命令列选项说明\n");
		printf("            -z  直接进入保护模式\n");
		printf("            -d  观看除错资讯\n");
		printf("            N   直接跳至第 N 个选单\n");
		printf("            Nx  直接连接 Nx 选项\n\n");
	    }
	    if (!callkey)
		exit(1);
	} else if (isdigit(argv[1][0])) {
	    s = argv[1][0] - '0';
	    if (s > secs)
		s = 1;
	    n = read_section(loc[s]);
	    if (argv[1][1] == '\0')
		goto quickmenu;
	    else if (argv[1][1] >= 'a')
		c = argv[1][1] - 'a';
	    else if ((argv[1][1] <= 'Z') && (argv[1][1] >= 'A'))
		c = 26 + argv[1][1] - 'A';
	    else
		c = 0;

	    if (c >= n)
		c = 0;

	    if (!info(c)) {
		printf("\n%s\n\n", command);
		exit(1);
	    }
	    ansi_clear;
	    ansi_cprint(WHITE, Header "\n\n");
	    ansi_cprint(LIGHTGREEN, "o 连往: %s (%s)\n", bbs[c].name, bbs[c].hostname);
	    ansi_cprint(LIGHTGREEN, "o 连不上时, 请按 Ctrl-C 离开\n\n");
	    if (bbs[c].login[0])
		ansi_cprint(LIGHTCYAN, "o 请以 %s login\n\n", bbs[c].login);
	    ansi_forecolor(LIGHTGRAY);
	    ansi_flush();
	    idle_time += time(NULL) - now_time;
	    now_time = time(NULL);
	    exec_wait(command, 0);
	    log(LOG_CONNECT, now_time, bbs[c].hostname, NULL);
	    connect_time += time(NULL) - now_time;
	    now_time = time(NULL);
	    Welcome();
	}
    }
quickmenu:

    n = read_section(loc[s]);

    time(&last_key_time);

    initscr();
    raw();
/*    cbreak();*/
    nonl();
    noecho();
    keypad(stdscr, TRUE);
    refresh();

    signal(SIGQUIT, Bye);	/* Ctrl-\ */
    signal(SIGINT, Bye);	/* Ctrl-C */
    signal(SIGTERM, Bye);	/* Terminate */
    signal(SIGTSTP, SIG_IGN);	/* Ctrl-Z */
    /* signal(SIGCONT,CtrlZ); */
    signal(SIGALRM, Clock);

    Clock(0);

    ansi_normal();
    ansi_forecolor(LIGHTGRAY);
    drawbox2(0, 1, COLS - 1, LINES - 2);
    ansi_print_xy(LINES - 5, 2, divide_line);

    mainmenu(s, secs);
    ansi_cprint_xy(0, 0, WHITE, "  ◎ %s ◎\0", Title[s]);

    H = LINES - 7;
    menu();

    c = 0;
    do {
	ansi_forecolor(WHITE);
	ansi_print_xy(LINES - 4, 2,
		      "单位:                 站名:                                                ");
	ansi_print_xy(LINES - 3, 2,
		      "连往:                                                                      ");

	sel_label[1] = symbol[c];

	ansi_ccprint_xy(x[c], y[c] - 1, LIGHTGREEN, BLUE, sel_label);
	ansi_ccprint_xy(x[c], y[c] + 2, LIGHTCYAN, BLUE, bbs[c].name);

	ansi_forecolor(YELLOW);
	ansi_backcolor(BLACK);

	ansi_print_xy(LINES - 4, 8, bbs[c].name);
	ansi_print_xy(LINES - 4, 30, bbs[c].alias);

	ansi_print_xy(LINES - 3, 8, bbs[c].hostname);
	if (bbs[c].port != 23)
	    ansi_print(" %d", bbs[c].port);

	if (bbs[c].login[0] != '\0')
	    ansi_print_xy(LINES - 4, 52, "login: %s", bbs[c].login);

	if (!info(c))
	    HelpBar(LIGHTRED, command);

	ansi_flush();
	if (callkey == 0)
	    key = getch();
	callkey = 0;
	if (key != ERR)		/* Normal key */
	    time(&last_key_time);
	else {			/* alarm() side-effect */
	    time(&now);
	    if (now - last_key_time >= offscreen) {
		key = '!';
	    }
	}

	label[1] = symbol[c];
	ansi_cprint_xy(x[c], y[c] - 1, LIGHTGREEN, label);
	ansi_cprint_xy(x[c], y[c] + 2, LIGHTGRAY, bbs[c].name);

	ansi_flush();
	switch (key) {
	  case 27:
	    HelpBar(LIGHTMAGENTA, "你按了 [Esc] 键, 若要离开请再按一次 ..");
	    ansi_flush();
	    key = getch();
	    if ((key == 'O') || (key == '['))
		key = getch();
	    callkey = 1;
	    if (key != 27) {
		switch (key) {
		  case 'A':	/* Up */
		  case 'i':
		    key = KEY_UP;
		    break;
		  case 'B':	/* Down */
		    key = KEY_DOWN;
		    break;
		  case 'C':	/* Right */
		    key = KEY_RIGHT;
		    break;
		  case 'D':	/* Left */
		    key = KEY_LEFT;
		    break;
		  case 'I':	/* ansi  PgUp */
		    /* case 'V':	 at386 PgUp */
		    /* case 'S':	 97801 PgUp */
		    /* case 'v':	 emacs style */
		    key = 32;
		    break;
		  case 'G':	/* ansi  PgDn */
		    /* case 'U':	 at386 PgDn */
		    /* case 'T':	 97801 PgDn */
		    key = 127;
		    break;
		  case 'H':	/* at386  Home */
		    key = '^';
		    break;
		  case 'w':
		  case 'F':	/* ansi   End */
		    /* case 'Y':	 at386  End */
		    key = '$';
		    break;
		  case '5':	/* vt200 PgUp */
		    key = 127;
		    getch();
		    break;
		  case '6':	/* vt200 PgDn */
		    key = 32;
		    getch();
		    break;
		  case '1':	/* vt200 Home */
		    key = '^';
		    getch();
		    break;
		  case '4':	/* vt200 End */
		    key = '$';
		    getch();
		    break;
		  default:
		    callkey = 0;
		}

	    } else {
		key = 3;	/* One more Esc to QUIT */
	    }
	    HelpBar(LIGHTCYAN, helpbar[showbar]);
	    break;
	  case KEY_LEFT:
	    c -= H;
	    if (c < 0)
		c = c + H * (int) ((n - 1) / H + 1);
	    if (c >= n)
		c -= H;
	    break;
	  case KEY_UP:
	    if (c > 0)
		c--;
	    else
		c = n - 1;
	    break;
	  case KEY_RIGHT:
	    c += H;
	    if (c >= n)
		c = c % H;
	    break;
	  case KEY_DOWN:
	    c = (c + 1) % n;
	    break;
	  case '^':
	    c = 0;
	    break;
	  case '$':
	  case 273:
	    c = n - 1;
	    break;
	  case 32:		/* Space */
	  case 338:		/* xterm PgDn */
	    s = (s % secs) + 1;

	    n = read_section(loc[s]);
	    if (c >= n)
		c = n - 1;
	    redraw();
	    break;
	  case 127:		/* Backspace */
	  case 8:
	  case 339:		/* xterm PgUp */
	    if (s-- == 1)
		s = secs;

	    n = read_section(loc[s]);
	    if (c >= n)
		c = n - 1;
	    redraw();
	    break;
	  case '?':
	    alarm(60);
	    Help();
	    alarm(30);
	  case '/':
	    redraw();
	    break;
	  case 20:		/* Ctrl-T */
	    TOGGLE(DEF_SHOWANSI);
	    ansi_show();
	    redraw();
	    break;
	  case '!':
	  case 26:
	    alarm(0);
	    if (DEF(DEF_ETENMODE))
		printf("\033iM;");
	    ansi_clear;
	    ansi_flush();
	    Saver(0);
	    if (DEF(DEF_ETENMODE))
		printf("\033iMim;");
	    redraw();
	    break;
	  case '\r':
	  case 1943:		/* IBM AIX */
	    if (command[0] == '[')
		break;
	    leave();
	    ansi_cprint_xy(0, 0, WHITE, Header "\n\n");
	    ansi_cprint(LIGHTGREEN, "o 连往: %s (%s)\n", bbs[c].name, bbs[c].hostname);
	    ansi_cprint(LIGHTGREEN, "o 连不上时，请按 Ctrl-C 离开\n\n");

	    if (bbs[c].login[0])
		ansi_cprint(LIGHTCYAN, "o 请以 %s login\n\n", bbs[c].login);
	    ansi_forecolor(LIGHTGRAY);

	    ansi_flush();
	    idle_time += time(NULL) - now_time;
	    now_time = time(NULL);
	    exec_wait(command, 1);
	    log(LOG_CONNECT, now_time, bbs[c].hostname, NULL);
	    connect_time += time(NULL) - now_time;
	    last_key_time = now_time = time(NULL);
	    Init();
	    redraw();
	    break;
	  case 22:		/* Ctrl-V */
	    HelpBar(LIGHTRED, "⊙ 初版: " BBSfirst "  本版: " BBSdate " (" BBSvers ") ⊙");
	    break;
	  default:
	    if (!DEF(DEF_SECURITY)) {
		int kk;
		switch (key) {
		  case 5:
		    alarm(0);
		    if (cfg != 'u') {
			HelpBar(LIGHTCYAN, "您并无自己的设定档，是否在您的目录下存放一个 ? (y/n)");
			ansi_flush();
			strcpy(command, "/bin/test");
			kk = getch();
			if (toupper(kk) == 'Y') {
			    if (cfg == 's')
				sprintf(command, "/bin/cp %s %s",
					DEF_SYS_BBSDATA, usercfg);
#if defined(BBSDATA)
			    else
				sprintf(command, "/bin/cp %s %s",
					BBSDATA, usercfg);
#endif
			    exec_wait(command, 1);
			} else {
			    HelpBar(LIGHTGREEN, "不使用自己的设定档 - 无法编辑");
			    alarm(5);
			    break;
			}
		    }
		    if (stat(usercfg, &file_status) < 0) {
			HelpBar(LIGHTRED, "[错误] 无法复制资料档");
			alarm(5);
			break;
		    }
		    sprintf(command, "%s %s", Editor, usercfg);
		    leave();
		    exec_wait(command, 1);
		    Bye(0);
		    break;
		  case '\t':
		  case 16:	/* Ctrl-P */
		    if (hosttype == Host_BBS) {
			leave();
			MailPost(c);
			Init();
			redraw();
			HelpBar(LIGHTRED, "Mailpost - 信件已经寄出");
		    }
		    break;
		  case 21:	/* Ctrl-U */
		    if (hosttype == Host_BBS) {
			leave();
			Mail2user(c);
			Init();
			redraw();
			HelpBar(LIGHTRED, "信件已经寄出给此站内的使用者");
		    }
		    break;
		  case 2:	/* Ctrl-B */
		    if (DEF(DEF_MAILSERV) && Ask("Source"))
			HelpBar(LIGHTRED, "[取得原始程式] 原始程式码将在一天内寄给您");
		    break;
/*                                 Ctrl-F
		  case 6:
		    if (DEF(DEF_MAILSERV) && Ask("Format"))
			HelpBar(LIGHTRED, "[查询资料档格式] 资料设定档将在一天内寄给您");
		    break;
*/
		  case 1:	/* Ctrl-A */
		    if (!DEF(DEF_MAILSERV))
			break;
		    leave();
		    Mail2author();
		    Init();
		    redraw();
		    HelpBar(LIGHTRED, "信件已经送出给作者");
		    break;
		}
	    }
	}
	if ((key <= '9') && (key >= '1') && (s != (key - '0'))) {
	    s = key - '0';
	    if (s > secs)
		s = 1;
	    n = read_section(loc[s]);
	    if (c >= n)
		c = n - 1;
	    redraw();
	}
	if ((key <= 'z') && (key >= 'A')) {
	    if (key >= 'a')
		c = key - 'a';
	    else if ((key <= 'Z') && (key >= 'A'))
		c = 26 + key - 'A';
	    if (c >= n)
		c = 0;
	}
    } while (key != 3 && key != 4);


    leave();

    Welcome();
}
