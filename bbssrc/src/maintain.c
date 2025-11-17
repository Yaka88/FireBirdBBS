/*
    Pirate Bulletin Board System
    Copyright (C) 1990, Edward Luke, lush@Athena.EE.MsState.EDU
    Eagles Bulletin Board System
    Copyright (C) 1992, Raymond Rocker, rocker@rock.b11.ingr.com
                        Guy Vega, gtvega@seabass.st.usm.edu
                        Dominic Tynes, dbtynes@seabass.st.usm.edu
    Firebird Bulletin Board System
    Copyright (C) 1996, Hsien-Tsung Chang, Smallpig.bbs@bbs.cs.ccu.edu.tw
                        Peng Piaw Foong, ppfoong@csie.ncu.edu.tw

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/*
$Id: maintain.c,v 1.1 2000/01/15 01:45:28 edwardc Exp $
*/

#include "bbs.h"

char   *Ctime();
extern char    cexplain[STRLEN];
extern char    lookgrp[30];
extern FILE   *cleanlog;

int
check_systempasswd()
{
	FILE   *pass;
	char    passbuf[20], prepass[STRLEN];
	clear();
	if ((pass = fopen("etc/.syspasswd", "r")) != NULL) {
		fgets(prepass, STRLEN, pass);
		fclose(pass);
		prepass[strlen(prepass) - 1] = '\0';
		getdata(1, 0, "������ϵͳ����: ", passbuf, 19, NOECHO, YEA);
		if (passbuf[0] == '\0' || passbuf[0] == '\n')
			return NA;
		if (!checkpasswd(prepass, passbuf)) {
			move(2, 0);
			prints("�����ϵͳ����...");
			securityreport("ϵͳ�����������...");
			pressanykey();
			return NA;
		}
	}
	return YEA;
}

int
securityreport(str)
char   *str;
{
	FILE   *se;
	char    fname[STRLEN];
	int     savemode;
	savemode = uinfo.mode;
	report(str);
	sprintf(fname, "tmp/security.%s.%05d", currentuser.userid, uinfo.pid);
	if ((se = fopen(fname, "w")) != NULL) {
		fprintf(se, "ϵͳ��ȫ��¼\n[1mԭ��%s[m\n", str);
		fprintf(se, "�����Ǹ�������");
		getuinfo(se);
		fclose(se);
		postfile(fname, "syssecurity", str, 2);
		unlink(fname);
		modify_user_mode(savemode);
	}
}

int
get_grp(seekstr)
char    seekstr[STRLEN];
{
	FILE   *fp;
	char    buf[STRLEN];
	char   *namep;
	if ((fp = fopen("0Announce/.Search", "r")) == NULL)
		return 0;
	while (fgets(buf, STRLEN, fp) != NULL) {
		namep = strtok(buf, ": \n\r\t");
		if (namep != NULL && ci_strcmp(namep, seekstr) == 0) {
			fclose(fp);
			strtok(NULL, "/");
			namep = strtok(NULL, "/");
			if (strlen(namep) < 30) {
				strcpy(lookgrp, namep);
				return 1;
			} else
				return 0;
		}
	}
	fclose(fp);
	return 0;
}

void
stand_title(title)
char   *title;
{
	clear();
	standout();
	prints(title);
	standend();
}

int
valid_brdname(brd)
char   *brd;
{
	char    ch;
	ch = *brd++;
	if (!isalnum(ch) && ch != '_')
		return 0;
	while ((ch = *brd++) != '\0') {
		if (!isalnum(ch) && ch != '_')
			return 0;
	}
	return 1;
}

char   *
chgrp()
{
	int     i, ch;
	char    buf[STRLEN], ans[6];

	static char *explain[] = {
		"��վϵͳ",
		"�㽭��ѧ",
		"���Լ���",
		"ѧ����ѧ",
		"�����Ļ�",
		"����У԰",
		"��������",
		"֪�Ը���",
		"����ʱ��",
		NULL
	};

	static char *groups[] = {
		"GROUP_0",
		"GROUP_1",
		"GROUP_2",
		"GROUP_3",
		"GROUP_4",
		"GROUP_5",
		"GROUP_6",
		"GROUP_7",
		"GROUP_8",
		NULL
	};
	clear();
	move(2, 0);
	prints("ѡ�񾫻�����Ŀ¼\n\n");
	for (i = 0;; i++) {
		if (explain[i] == NULL || groups[i] == NULL)
			break;
		prints("[1;32m%2d[m. %-20s%-20s\n", i, explain[i], groups[i]);
	}
	sprintf(buf, "���������ѡ��(0~%d): ", --i);
	while (1) {
		getdata(i + 6, 0, buf, ans, 4, DOECHO, YEA);
		if (!isdigit(ans[0]))
			continue;
		ch = atoi(ans);
		if (ch < 0 || ch > i || ans[0] == '\r' || ans[0] == '\0')
			continue;
		else
			break;
	}
	sprintf(cexplain, "%s", explain[ch]);

	return groups[ch];
}

char    curruser[IDLEN + 2];
extern int delmsgs[];
extern int delcnt;

void
domailclean(fhdrp)
struct fileheader *fhdrp;
{
	static int newcnt, savecnt, deleted, idc;
	char    buf[STRLEN];
	if (fhdrp == NULL) {
		fprintf(cleanlog, "new = %d, saved = %d, deleted = %d\n",
			newcnt, savecnt, deleted);
		newcnt = savecnt = deleted = idc = 0;
		if (delcnt) {
			sprintf(buf, "mail/%c/%s/%s", toupper(curruser[0]), curruser, DOT_DIR);
			while (delcnt--)
				delete_record(buf, sizeof(struct fileheader), delmsgs[delcnt]);
		}
		delcnt = 0;
		return;
	}
	idc++;
	if (!(fhdrp->accessed[0] & FILE_READ))
		newcnt++;
	else if (fhdrp->accessed[0] & FILE_MARKED)
		savecnt++;
	else {
		deleted++;
		sprintf(buf, "mail/%c/%s/%s", toupper(curruser[0]), curruser, fhdrp->filename);
		unlink(buf);
		delmsgs[delcnt++] = idc;
	}
}

int
cleanmail(urec)
struct userec *urec;
{
	struct stat statb;
	if (urec->userid[0] == '\0' || !strcmp(urec->userid, "new"))
		return 0;
	sprintf(genbuf, "mail/%c/%s/%s", toupper(urec->userid[0]), urec->userid, DOT_DIR);
	fprintf(cleanlog, "%s: ", urec->userid);
	if (stat(genbuf, &statb) == -1)
		fprintf(cleanlog, "no mail\n");
	else if (statb.st_size == 0)
		fprintf(cleanlog, "no mail\n");
	else {
		strcpy(curruser, urec->userid);
		delcnt = 0;
		apply_record(genbuf, domailclean, sizeof(struct fileheader));
		domailclean(NULL);
	}
	return 0;
}


void
trace_state(flag, name, size)
int     flag, size;
char   *name;
{
	char    buf[STRLEN];
	if (flag != -1) {
		sprintf(buf, "ON (size = %d)", size);
	} else {
		strcpy(buf, "OFF");
	}
	prints("%s��¼ %s\n", name, buf);
}


int
scan_register_form(regfile)
char   *regfile;
{
	static char *field[] = {"usernum", "userid", "realname", "dept",
	"addr", "phone", "assoc", NULL};
	static char *finfo[] = {"�ʺ�λ��", "�����ʺ�", "��ʵ����", "ѧУϵ��",
	"Ŀǰסַ", "����绰", "У �� ��", NULL};
	static char *reason[] = {"��ȷʵ��д��ʵ����.", "������ѧУ��ϵ���꼶.",
		"����д������סַ����.", "����������绰.",
		"����дУ�ѻ�����.", "��ȷʵ��дע�������.",
		"����������д���뵥.", "���� Mail Reply ��ʽע��",
	"���ü���email���mail reply", NULL};
	struct userec uinfo;
	FILE   *fn, *fout, *freg;
	char    fdata[7][STRLEN];
	char    fname[STRLEN], buf[STRLEN];
	char    ans[5], *ptr, *uid;
	int     n, unum;
	uid = currentuser.userid;
	stand_title("�����趨������ע������");
	sprintf(fname, "%s.tmp", regfile);
	move(2, 0);
	if (dashf(fname)) {
		move(1, 0);
		prints("���� SYSOP ���ڲ鿴ע�����뵥, ����ʹ����״̬.\n");
		getdata(2, 0, "��ȷ��û������ SYSOP �����ע�ᵥ�� �� [y/N]: ", ans, 2, DOECHO, YEA);
		if (ans[0] == 'Y' || ans[0] == 'y')
			f_cp(fname, regfile, O_APPEND);
		else {
			pressreturn();
			return -1;
		}
	}
	rename(regfile, fname);
	if ((fn = fopen(fname, "r")) == NULL) {
		move(2, 0);
		prints("ϵͳ����, �޷���ȡע�����ϵ�: %s\n", fname);
		pressreturn();
		return -1;
	}
	memset(fdata, 0, sizeof(fdata));
	while (fgets(genbuf, STRLEN, fn) != NULL) {
		if ((ptr = (char *) strstr(genbuf, ": ")) != NULL) {
			*ptr = '\0';
			for (n = 0; field[n] != NULL; n++) {
				if (strcmp(genbuf, field[n]) == 0) {
					strcpy(fdata[n], ptr + 2);
					if ((ptr = (char *) strchr(fdata[n], '\n')) != NULL)
						*ptr = '\0';
				}
			}
		} else if ((unum = getuser(fdata[1])) == 0) {
			move(2, 0);
			clrtobot();
			prints("ϵͳ����, ���޴��ʺ�.\n\n");
			for (n = 0; field[n] != NULL; n++)
				prints("%s     : %s\n", finfo[n], fdata[n]);
			pressreturn();
			memset(fdata, 0, sizeof(fdata));
		} else {
			memcpy(&uinfo, &lookupuser, sizeof(uinfo));
			move(1, 0);
			prints("�ʺ�λ��     : %d\n", unum);
			disply_userinfo(&uinfo, 1);
			move(15, 0);
			printdash(NULL);
			for (n = 0; field[n] != NULL; n++)
				prints("%s     : %s\n", finfo[n], fdata[n]);
			if (uinfo.userlevel & PERM_LOGINOK) {
				move(t_lines - 1, 0);
				prints("���ʺŲ�������дע�ᵥ.\n");
				igetkey();
				ans[0] = 'D';
			} else {
				getdata(t_lines - 1, 0, "�Ƿ���ܴ����� (Y/N/Q/Del/Skip)? [S]: ",
					ans, 3, DOECHO, YEA);
			}
			move(1, 0);
			clrtobot();
			switch (ans[0]) {
			case 'D':
			case 'd':
				break;
			case 'Y':
			case 'y':
				prints("����ʹ���������Ѿ�����:\n");
				n = strlen(fdata[5]);
				if (n + strlen(fdata[3]) > 60) {
					if (n > 40)
						fdata[5][n = 40] = '\0';
					fdata[3][60 - n] = '\0';
				}
				strncpy(uinfo.realname, fdata[2], NAMELEN);
				strncpy(uinfo.address, fdata[4], NAMELEN);
				sprintf(genbuf, "%s$%s@%s", fdata[3], fdata[5], uid);
				genbuf[STRLEN - 16] = '\0';
				strncpy(uinfo.reginfo, genbuf, STRLEN - 17);
				uinfo.lastjustify = time(0);
				substitute_record(PASSFILE, &uinfo, sizeof(uinfo), unum);
				sethomefile(buf, uinfo.userid, "register");
				if (dashf(buf)) {
					sethomefile(genbuf, uinfo.userid, "register.old");
					rename(buf, genbuf);
				}
				if ((fout = fopen(buf, "w")) != NULL) {
					for (n = 0; field[n] != NULL; n++)
						fprintf(fout, "%s: %s\n", field[n], fdata[n]);
					n = time(NULL);
					fprintf(fout, "Date: %s\n", Ctime(&n));
					fprintf(fout, "Approved: %s\n", uid);
					fclose(fout);
				}
				mail_file("etc/s_fill", uinfo.userid, "�����㣬���Ѿ����ע�ᡣ");
				sethomefile(buf, uinfo.userid, "mailcheck");
				unlink(buf);
				sprintf(genbuf, "�� %s ͨ������ȷ��.", uinfo.userid);
				securityreport(genbuf);
				break;
			case 'Q':
			case 'q':
				if ((freg = fopen(regfile, "a")) != NULL) {
					for (n = 0; field[n] != NULL; n++)
						fprintf(freg, "%s: %s\n", field[n], fdata[n]);
					fprintf(freg, "----\n");
					while (fgets(genbuf, STRLEN, fn) != NULL)
						fputs(genbuf, freg);
					fclose(freg);
				}
				break;
			case 'N':
			case 'n':
				for (n = 0; field[n] != NULL; n++)
					prints("%s: %s\n", finfo[n], fdata[n]);
				printdash(NULL);
				move(9, 0);
				prints("��ѡ��/�����˻������ԭ��, �� <enter> ȡ��.\n\n");
				for (n = 0; reason[n] != NULL; n++)
					prints("%d) %s\n", n, reason[n]);
				getdata(12 + n, 0, "�˻�ԭ��: ", buf, 60, DOECHO, YEA);
				if (buf[0] != '\0') {
					if (buf[0] >= '0' && buf[0] < '0' + n) {
						strcpy(buf, reason[buf[0] - '0']);
					}
					sprintf(genbuf, "<ע��ʧ��> - %s", buf);
					strncpy(uinfo.address, genbuf, NAMELEN);
					substitute_record(PASSFILE, &uinfo, sizeof(uinfo), unum);
					mail_file("etc/f_fill", uinfo.userid, uinfo.address);
					/* user_display( &uinfo, 1 ); */
					/* pressreturn(); */
					break;
				}
				move(10, 0);
				clrtobot();
				prints("ȡ���˻ش�ע�������.\n");
				/* run default -- put back to regfile */
			default:
				if ((freg = fopen(regfile, "a")) != NULL) {
					for (n = 0; field[n] != NULL; n++)
						fprintf(freg, "%s: %s\n", field[n], fdata[n]);
					fprintf(freg, "----\n");
					fclose(freg);
				}
			}
			memset(fdata, 0, sizeof(fdata));
		}
	}
	fclose(fn);
	unlink(fname);
	return (0);
}

