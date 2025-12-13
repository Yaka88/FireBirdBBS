
/* BBS version 1.8            */
/* First version: Oct 1,1994  */

#define BBSver      "v1.8"
#define Patch       "1"
#define BBSdate     "Apr 04,1997"
#define BBSfirst    "Oct 01,1994"
#define BBSvers     BBSver""Patch
#define CFGFILE     "bbsdata-"BBSver
#define BBS_Max     52
#define Rec_Len     85
#define Header      "bbs "BBSvers"   ＝ 网路资源连线程式 ＝"
#define DEF_SYS_BBSDATA     "/usr/local/etc/"CFGFILE
#define MAIN_MENU	(9)
#define MAILCHKBEEP     (5)


#ifdef BSD

#	if defined(__386BSD__) || defined(__bsdi__) || defined(__NetBSD__) || defined(__FreeBSD__)
#		define	DEFAULT_MAILBOX "/var/mail"
#	endif

#	ifndef DEFAULT_MAILBOX
#		define	DEFAULT_MAILBOX "/usr/spool/mail"
#	endif

#else

#	ifdef linux
#		define	DEFAULT_MAILBOX "/usr/spool/mail"
#	endif

#	ifndef DEFAULT_MAILBOX
#		define	DEFAULT_MAILBOX "/usr/mail"
#	endif

#endif

/*
#define BSD_MAILSPOOL  "/var/mail/"
#define SYSV_MAILSPOOL "/var/spool/mail/"
#define MAILBOX1     SYSV_MAILSPOOL
#define MAILBOX2     BSD_MAILSPOOL
*/

#if defined(NCTUCC)
#define BBSDATA    "/afs/cc.nctu.edu.tw/users/ie/85G/u8517504/Bin/."CFGFILE
#elif defined(NCTUCSIE)
#define BBSDATA    "/u/gcp/85/8517504/."CFGFILE
#elif defined(USERDEF)
#define BBSDATA    USERDEF
#endif

#if defined(LINUX)
#include <ncurses.h>
#elif defined(FREEBSD)
#include <ncurses.h>
#elif defined(ULTRIX)
#include <cursesX.h>
#else
#include <curses.h>
#endif

#include <time.h>

/* Host structure */

struct BBS {
    char name[18];		/* 单位 */
    char alias[20];		/* 站名 */
    char hostname[60];		/* 位址 */
    int port;			/* port */
    char login[20];		/* login name */
};
#define Host_TEL	(char)0	/* 单纯 telnet */
#define Host_BBS	(char)1	/* 可作 Mailpost/Mail-to-user  */
#define Host_GOPHER	(char)2	/* 会试著以 Gopher Client 连线 */
#define Host_FTP	(char)3	/* 以 FTP client 连上 */
#define Host_WWW	(char)4	/* disabled now */

#define AUTHOR		"cdsheen@csie.nctu.edu.tw"

extern void print_debug(FILE*);
extern void Welcome(void);
extern void Bye(int);
extern void drawbox1(int, int, int, int);
extern void drawbox2(int, int, int, int);
extern void MailPost(char);
extern void Mail2user(char);
extern void Mail2author(void);
extern void Help(void);
extern char *Edit(void);
extern void Delete(void);
extern int read_section(long);
extern void menu(void);
extern void mainmenu(int, int);
extern int exec_wait(char *, int);

extern void log(int, time_t, char *, char *);

/* user define stuff */
#define DEF_MAILCHECK	0x0001
#define DEF_COUNTTIME	0x0002
#define DEF_MAILSERV	0x0004
#define DEF_SECURITY	0x0008
#define DEF_ETENMODE	0x0010
#define DEF_SHOWANSI	0x0020
#define DEF_LOGGING	0x0040

#define DEF(v)		( mode&v )
#define SETMODE(v,x)	( (x) ? (mode |= v) : ( mode&=~v ) )
#define TOGGLE(v)	mode ^= v

/* log 项目 */
#define LOG_CONNECT	0
#define LOG_EXITPROG	1
#define LOG_MAILPOST	2
#define LOG_MAILUSER	3
#define LOG_MAILAUTH	4

/* ANSI I/O function 之 buffer 大小 */
#define ANSI_BUF_MAX	4000

/* ANSI I/O function 所接受之每行最大字元数 */
#define ANSI_LINEMAX	200

/* 颜色定义 */

#define BLACK		((unsigned char)0)
#define BLUE		((unsigned char)1)
#define	GREEN		((unsigned char)2)
#define CYAN		((unsigned char)3)
#define RED		((unsigned char)4)
#define MAGENTA		((unsigned char)5)
#define	BROWN		((unsigned char)6)
#define	LIGHTGRAY	((unsigned char)7)

#define DARKGRAY	((unsigned char)8)
#define LIGHTBLUE	((unsigned char)9)
#define LIGHTGREEN	((unsigned char)10)
#define LIGHTCYAN	((unsigned char)11)
#define LIGHTRED	((unsigned char)12)
#define	LIGHTMAGENTA	((unsigned char)13)
#define YELLOW		((unsigned char)14)
#define WHITE		((unsigned char)15)

/* 清除萤幕 */
#define ansi_clear		ansi_bufs("\033[2J\033[1;1H")

/* extern void ansi_buf(int); */
extern void ansi_show(void);
extern void ansi_bufs(char *);
extern void ansi_flush(void);
extern void ansi_color(unsigned char fore, unsigned char back);
extern void ansi_normal(void);

extern void ansi_print(char *,...);
extern void ansi_cprint(unsigned char, char *,...);
extern void ansi_print_xy(int x, int y, char *,...);
extern void ansi_cprint_xy(int, int, unsigned char, char *,...);
extern void ansi_ccprint_xy(int, int, unsigned char, unsigned char, char *,...);
