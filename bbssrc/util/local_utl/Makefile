# Generated automatically from Makefile.in by configure.
#  Makefile for local utility tool kit
# 
#  $Id: Makefile.in,v 1.2 2000/03/07 10:41:07 skyo Exp $
#

CC       = gcc
CFLAGS   = -g -O2 -I../../include
BBSHOME	 = /home/runner/work/FireBirdBBS/FireBirdBBS

UTILITY  = account averun bbstop bbspop3d bfinger birthday horoscope \
		   newacct poststat remake usage showuser
#disable http-finger
#		   http-finger newacct poststat remake usage showuser

.c.o:   ;   $(CC) $(CFLAGS) -c $*.c

all: $(UTILITY)

install: $(UTILITY)
	cp $(UTILITY) $(BBSHOME)/bin

clean:
	rm -fr *.o *.BAK *.log $(UTILITY) *core* DEAD* *~

bbspop3d:
	$(CC) $(CFLAGS) -o bbspop3d bbspop3d.c -lcrypt

bfinger: bfinger.c ../../src/modetype.c 
	$(CC) $(CFLAGS) -o bfinger bfinger.c ../../src/modetype.c

http-finger: http-finger.c ../../src/modetype.c 
	$(CC) $(CFLAGS) -o http-finger http-finger.c ../../src/modetype.c

remake: remake.c ../../src/record.c
	$(CC) $(CFLAGS) -o remake remake.c ../../src/record.c

usage: usage.c ../../src/record.c
	$(CC) $(CFLAGS) -o usage usage.c ../../src/record.c
