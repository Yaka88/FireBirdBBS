
#include <stdio.h>
#include "config.h"

extern struct BBS bbs[BBS_Max];
extern long loc[MAIN_MENU + 1];
extern int hosttype;

int read_section(long foff)
{

    char buffer[Rec_Len + 1];
    char nouse[30];
    int s, i, p;
    int port;
    char type[15];
    char *login;
    extern FILE *fp;

    fseek(fp, foff, SEEK_SET);

    fgets(buffer, Rec_Len, fp);
    sscanf(buffer, "* %s %s", nouse, type);

    if (!strncasecmp(type, "BBS", 3))
	hosttype = Host_BBS;
    else if (!strncasecmp(type, "GOPHER", 6))
	hosttype = Host_GOPHER;
    else if (!strncasecmp(type, "TEL", 3))
	hosttype = Host_TEL;
    else if (!strncasecmp(type, "FTP", 3))
	hosttype = Host_FTP;

    port = i = 0;
    s = 1;
    while (fgets(buffer, Rec_Len, fp) != NULL) {
	switch (buffer[0]) {
	  case '\0':
	  case '#':
	  case ' ':
	  case '\t':
	  case '\n':
	  case '\r':
	    break;
	  case '*':
	    if (!strncasecmp(&buffer[3], "End", 3))
		s = 0;
	    break;
	  default:
	    port = 0;
	    sscanf(buffer, "%s %s %s %d",
		   bbs[i].name, bbs[i].alias, bbs[i].hostname, &port);

	    bbs[i].port = (port == 0) ? 23 : port;

	    login = (char *) strchr(bbs[i].hostname, '@');

	    if (login == NULL)
		bbs[i].login[0] = '\0';
	    else {
		*login = '\0';
		strcpy(bbs[i].login, bbs[i].hostname);
		for (login++, p = 0; *login; login++, p++)
		    bbs[i].hostname[p] = *login;
		bbs[i].hostname[p] = '\0';
	    }
	    i++;
	}

	if (s == 0 || i == BBS_Max)
	    break;
    }

    return (i);
}
