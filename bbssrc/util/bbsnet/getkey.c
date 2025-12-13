
#include <termio.h>
#include <stdio.h>

int getch()
{
  struct termio save,term;
  char key;

  if(ioctl(0,TCGETA,&term)==-1){
	return(-1);
  }

  save=term;
  term.c_lflag&=~ICANON;
  term.c_lflag&=~ECHO;

  term.c_cc[VMIN]=1;
  term.c_cc[VTIME]=0;

  ioctl(0,TCSETA,&term);

  read(0,&key,1);

  ioctl(0,TCSETA,&save)  ;

  return((int)key);
}

