
/**************************************************

file: demo_rx.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"
#include "utils/utils.h"
#include "utils/iniparser.h"

int fdw = 0;

void intHandler(int signo) {
  close(fdw);
  printf("closing fdw\n");
  exit(0);
}

int main(int argc, char* argv[])
{
  dictionary *conf=NULL;
  int cport_nr=17,        /* /dev/ttyUSB1 */
      bdrate=115200;       /* 115200 baud */
  char mode[]={'8','N','1',0};

  unsigned char *buf=NULL;
  int bufSize=4*1024;
  int countr=0, totalr=0;

  if(signal(SIGINT, intHandler)==SIG_ERR)
  {
    printf("Unable to catch SIGINT\n");
  }

  /* Allocate buffer to receive */
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

  cport_nr = iniparser_getint(conf, "rx:cport_nr", 17);
  bdrate = iniparser_getint(conf, "rx:bdrate", 115200);
  strncpy(mode, iniparser_getstring(conf, "rx:mode", "8N1"), 4);
  printf("cport is %d\n", cport_nr);
  printf("rate is %d\n", bdrate);
  printf("mode is %c %c %c %c\n", mode[0],mode[1],mode[2],mode[3]);
  iniparser_freedict(conf);

  /* Open output file to write */
  if(argc>1)
  {
    fdw = open(argv[1], O_WRONLY | O_CREAT, 0644);
    if(fdw==0)
    {
      printf("Failed to open %s\n", argv[1]);
      return(0);
    }
  }
  else
  {
    fdw = 1;
  } 

  /* Open UART port */
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1)
  {
    countr = RS232_PollComport(cport_nr, buf, bufSize);
    totalr += countr;

    if(countr > 0)
    {
      write(fdw, buf, countr);
      //printf("received %i bytes. total received %i bytes.\n", countr, totalr);
    }

    delay(10);
  }

  return(0);
}

