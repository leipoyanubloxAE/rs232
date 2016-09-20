
/**************************************************

file: demo_tx.c
purpose: simple demo that transmits characters to
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_tx.c rs232.c -Wall -Wextra -o2 -o test_tx

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"
#include "utils/utils.h"
#include "utils/iniparser.h"

void send_buffer(int cport_nr, unsigned char* buf, int countr)
{
  int countw=0, totalw=0;

  while(totalw!=countr)
  {
    countw = RS232_SendBuf(cport_nr, buf+totalw, countr-totalw);
    totalw +=countw;
    if(totalw != countr)
    {
      delay(10);
    }
  }
}

int main(int argc, char* argv[])
{
  dictionary *conf=NULL;
  int cport_nr=16, 
      bdrate=115200;  
  char mode[]={'8','N','1',0};
  unsigned char *buf=NULL;
  int bufSize=(8*1024);
  int fdr = 0, countr=0, totalr=0;

  if(argc<2)
  {
    printf("Please specify the file to send\n");
    return(0);  
  }

  /* Open input file and allocate read buffer */ 
  printf("File to send is %s\n", argv[1]); 
  fdr = open(argv[1], O_RDONLY | O_SYNC);
  if(fdr==0)
  {
    printf("Failed to open %s\n", argv[1]);
    return(0);
  }  

  buf = malloc(sizeof(unsigned char)*bufSize);
  if(buf==NULL)
  {
    printf("Insufficient memory\n");
    return(0);
  }
  memset(buf, 0, bufSize);

  /* read uart setting from config file */
  conf = iniparser_load("uart.conf");
  if(conf==NULL)
  {
    printf("Faied to open config file uart.conf\n");
  }

  cport_nr = iniparser_getint(conf, "tx:cport_nr", 16);
  bdrate = iniparser_getint(conf, "tx:bdrate", 115200);
  strncpy(mode, iniparser_getstring(conf, "tx:mode", "8N1"), 4);
  printf("cport is %d\n", cport_nr);
  printf("rate is %d\n", bdrate);
  printf("mode is %c %c %c %c\n", mode[0],mode[1],mode[2],mode[3]);
  iniparser_freedict(conf);

  /* Open uart port */
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  do
  {
    countr = read(fdr, buf, bufSize);
    totalr += countr;
    printf("read %d bytes. Total sent %i bytes\n", countr, totalr);

AGAIN:
    if(RS232_IsCTSEnabled(cport_nr))
    {
      send_buffer(cport_nr, buf, countr);
    }
    else
    {
      delay(10);
      goto AGAIN;
    }

  delay(10);
  } while( countr > 0);

  return(0);
}

