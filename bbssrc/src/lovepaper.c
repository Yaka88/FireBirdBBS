/*===============================*/
/*==   ÇéÊé²úÉúÆ÷  Ptt         ==*/
/*===============================*/
#include "bbs.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define DATA "etc/lovepaper.dat"

extern void hold_mail(char *,char *);

int
x_love()
{
 char buf1[200],save_title[STRLEN+1],receiver[61],tmpf[STRLEN];
 int x,y,z,tline,poem=0,l=0;
 FILE *fp,*fpo;
 int internet_mail;
 time_t timenow;
 struct tm *gtime;

 receiver[0]=0;
 modify_user_mode(LOVE);
 //currentuser.money-=100;/*add by hightman for ÊÕ·Ñ */
 substitute_record(PASSFILE, &currentuser, sizeof(currentuser),usernum);
/* end */
 time(&timenow);
 srandom(timenow);
 gtime = localtime(&timenow);
 sprintf(tmpf,"/tmp/love%d%d",gtime->tm_sec,gtime->tm_min);
 move(0,0);
 clrtobot();

 outs("[1;33m»¶Ó­Ê¹ÓÃÇéÊé²úÉúÆ÷ - [m");
 outs("[1;32mÓĞºÎÄÑÒÔÆô³İµÄ»°,½»ÓÉÏµÍ³°ïÄãËµ°É. [m\n");
 
 usercomplete("¼Ä¸øÄÄÎ»ÇéÈË:", receiver);

  if ( receiver[0]==0 || !(searchuser(receiver) &&
    getdata(2, 0, "Ö÷   Ìâ:", save_title, 50, DOECHO,YEA)))
  {
    move(10, 0);
    outs("[1;33msigh...ÊÕĞÅÈË»òÖ÷Ìâ²»ÕıÈ·, [1;35mÇéÊé[mÎŞ·¨´«µİ.[m ");
    pressanykey(NULL);
    return 0;
  }

 fpo=fopen(tmpf,"w");
 fprintf(fpo,"\n");
 if((fp=fopen (DATA,"r")))
  {
   while( fgets(buf1,100,fp))
   {
    switch(buf1[0]) 
       {
	case '#':
	      break;
        case '@':
	    if (!strncmp(buf1,"@begin",6) || !strncmp(buf1,"@end",4))
		{
		 tline=3;
		}
	    else if(!strncmp(buf1,"@poem",5) )
		{
 		 poem=1;tline=1;
		 fprintf(fpo,"\n\n");
		}
            else 
		{
		tline=2;
		}
	    break;
	case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
	    sscanf(buf1,"%d",&x);
	    y=(random()%(x-1))*tline; 
	    break;
	default:
            if (!poem)
	     {
              if(y>0) {y=y-1;
}
	      else {
		    if(tline>0)
			{
			 fprintf(fpo,"%s",buf1);
			 tline--;
			}
		   }
	     }
	    else 
	     {
	      if(buf1[0]=='$') 
		{y--;}
	      else
		{ 		 			
 		   if(y==0) fprintf(fpo,"%s",buf1);
		}
	     }
	} 

   }

                          fclose(fp);
                          fclose(fpo);
    in_mail=YEA;
    if (vedit(tmpf, YEA) == -1)
    {
      unlink(tmpf);
      clear();
      outs("\n\n ËãÁË,·ÅÆú¼ÄÇéÊé\n");
      pressanykey(NULL);   	
      return -2;
    }                       

    mail_file(tmpf,receiver,save_title);
    unlink(tmpf);
    return 1;
  }
 return 0;
}
