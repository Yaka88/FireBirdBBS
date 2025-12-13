#################################################
#
# Makefile of BBS
#
#################################################

#CFG	= -DUSERDEF=\"/usr/etc/netdata\"
OPTS	= #-g

CC	= gcc

BINDIR	= /usr/local/bin
CFGDIR	= /usr/local/etc

#################################################

STRIP	= strip
MAKE	= make
RM	= /bin/rm -f
CP	= /bin/cp
RD	= rmdir
INDENT	= indent
SHAR	= shar
UU	= uuencode

VERSION	= v1.8

CFLAG	= -c -O2 $(OPTS) #-Wunused

OBJS	= bbs.o goodbye.o menu.o drawbox.o mailpost.o help.o log.o\
	section.o mailuser.o edit.o mailauth.o screen.o exec.o

FILES	= README *.c Makefile config.h bbsdata-v* CHANGES parsefunc .indent.pro

help:
	@echo
	@echo " make sunos   : for Sun-OS 4.x"
	@echo " make solaris : for Sun-OS 5.x (Solaris)"
	@echo " make linux   : for Linux 1.x 2.x"
	@echo " make aix     : for IBM AIX"
	@echo " make hp      : for HP UX"
	@echo " make freebsd : for FreeBSD"
	@echo

all:	help

.c.o:
	$(CC) $(CFLAG) $(INCDIR) $(ARCH) $(CFG) $*.c

sunos:
	@$(MAKE) LIBS="-lcurses -ltermcap"  LIBSDIR="-L/usr/5lib" INCDIR="-I/usr/5include" ARCH="-DSUNOS -DBSD" bbs
solaris:
	@$(MAKE) LIBS="-lcurses -ltermcap"  LIBSDIR="-L/usr/5lib" INCDIR="-I/usr/5include" ARCH="-DSUNOS -DSVR4" STRIP="/usr/ccs/bin/strip" bbs
linux:
	@$(MAKE) LIBS="-lncurses -ltermcap" INCDIR="-I/usr/include/ncurses" ARCH="-DLINUX" bbs
freebsd:
	@$(MAKE) LIBS="-lncurses" ARCH="-DFREEBSD -DBSD" bbs
hp:
	@$(MAKE) LIBS="-lcurses -ltermcap"  ARCH="-DHP -DSYSV" bbs
aix:
	@$(MAKE) LIBS="-lcurses -ltermcap"  ARCH="-DAIX -DSYSV" bbs
ultrix:
	@$(MAKE) LIBS="-lcursesX -ltermcap"  ARCH="-DULTRIX -DBSD" bbs

csie:
	@$(MAKE) LIBS="-lcurses -ltermcap"  LIBSDIR="-L/usr/5lib" INCDIR="-I/usr/5include" CFG=-DNCTUCSIE ARCH="-DSUNOS -DBSD" bbs
nctucc:
	@$(MAKE) LIBS="-lcurses -ltermcap"  LIBSDIR="-L/usr/5lib" INCDIR="-I/usr/5include" CFG=-DNCTUCC ARCH="-DSUNOS -DBSD" bbs


bbs:	$(OBJS) config.h
	@echo
	@echo Making executable binary --
	@echo
	$(CC) $(LIBSDIR) -o bbs $(OBJS) $(LIBS)
	@echo
	$(STRIP) bbs
	@echo
	@echo "bbs size: `cat bbs|wc -c` bytes"
	@echo

readkey:	readkey.c
	$(CC) -O2 $(INCDIR) $(LIBSDIR) $(ARCH) -o readkey readkey.c -lcurses

shar:	bbs.shar

bbs.shar:	$(FILES)
	@echo
	@echo Creating archive --
	@echo
	$(RM) bbs.shar
	$(SHAR) -o bbs.shar $(FILES)
	chmod 755 bbs.shar*
	@echo

indent:
	@echo
	@echo Formatting source codes --
	@echo
	for i in *.c *.h ;do\
		$(INDENT) $$i;\
	done
	$(RM) *.BAK
	@echo

tar:	bbs-$(VERSION).tar.gz

bbs-$(VERSION).tar.gz:	$(FILES)
	@echo
	test -d bbs-$(VERSION) || mkdir bbs-$(VERSION)
	touch $(FILES) bbs-$(VERSION)
	cp $(FILES) bbs-$(VERSION)
	tar zcf bbs-$(VERSION).tar.gz bbs-$(VERSION)
	$(RM) bbs-$(VERSION)/* bbs-$(VERSION)/.indent.pro
	$(RD) bbs-$(VERSION)

uu:	bbs-$(VERSION).uu

bbs-$(VERSION).uu:	bbs-$(VERSION).tar.gz
	$(RM) bbs-$(VERSION).uu
	$(UU) bbs-$(VERSION).tar.gz bbs-$(VERSION).tar.gz > bbs-$(VERSION).uu

install:	bbs
	$(CP)  bbs  $(BINDIR)
	$(CP)  bbsdata-v1.8 $(CFGDIR)

clean:
	@echo
	@echo Cleaning codes --
	@echo
	$(RM) *.o bbs core *~ bbs.shar bbs-$(VERSION).tar.gz readkey
	@echo

#
# Shen Cheng-Da (cdsheen@csie.nctu.edu.tw)  /NCTU CSIE/
#
