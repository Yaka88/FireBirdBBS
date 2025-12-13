
/* suggest by: leeym@bbs.civil.ncku.edu.tw */

#include <time.h>

#include "config.h"

void log(int type, time_t s_time, char *host, char *msg)
{
    FILE *fp;
    time_t now;

    int connect_time;
    struct tm *tms;
    char buf[200];
    char time_str[20];

    char log_str[][9] = {"CONNECT", "EXITPROG", "MAILPOST", "MAILUSER", "MAILAUTH"};

    extern char logfile[80];
    extern int mode;

/*
cdsheen      CONNECT  1996/10/10 12:30-13:50 (01:20) bbs.iljhs.il.edu.tw(bbs)
cdsheen      EXITPROG 1996/10/10 12:30-13:55 (01:25)
ylwu         MAILUSER 1996/10/11 12:40-17:41 (05:01) bbs.csie.nctu.edu.tw(id/subject)
knlau        MAILPOST 1996/10/11 12:40-17:41 (05:01) bbs.csie.nctu.edu.tw(board/subject)
*/

    if (!DEF(DEF_LOGGING))
	return;

    time(&now);
    connect_time = now - s_time;

    tms = localtime(&s_time);

    sprintf(time_str, "%04d/%02d/%02d %02d:%02d", tms->tm_year+1900,
	    tms->tm_mon + 1, tms->tm_mday, tms->tm_hour, tms->tm_min);

    tms = localtime(&now);

    sprintf(buf, "%-12s %-8s %s-%02d:%02d (%02d:%02d) ", getenv("USER"),
	    log_str[type], time_str, tms->tm_hour, tms->tm_min,
	    connect_time / 3600, (connect_time / 60) % 60);

    if (host)
	strcat(buf, host);

    if (msg) {
	strcat(buf, "(");
	strcat(buf, msg);
	strcat(buf, ")");
    }
    if (fp = fopen(logfile, "a")) {	/* 若无法开启，则不纪录 */
	fprintf(fp, "%s\n", buf);
	fclose(fp);
    }
}
