
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

int fdw = 0;

void intHandler(int dummy) {
  close(fdw);
  printf("closing fdw\n");
  exit(0);
}

int main()
{
  int cport_nr=17,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=3000000;       /* 9600 baud */

  char mode[]={'8','N','1',0};
  char* outputFilename = "out.out";

  unsigned char *buf=NULL;
  int bufSize=4*1024;
  int countr=0, totalr=0;

  signal(SIGINT, intHandler);

  fdw = open(outputFilename, O_WRONLY | O_CREAT, 0644);
  if(fdw==0)
  {
    printf("Failed to open %s\n", outputFilename);
    return(0);
  } 

  buf = malloc(sizeof(unsigned char)*bufSize);
  if(buf==NULL)
  {
    printf("Insufficient memory\n");
    return(0);
  }
  memset(buf, 0, bufSize);

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
      //write(1, buf, countr);
      //printf("received %i bytes. total received %i bytes.\n", countr, totalr);
    }

#if 1
#ifdef _WIN32
    Sleep(10);
#else
    usleep(10000);  /* sleep for 100 milliSeconds */
#endif
#endif
  }

  return(0);
}

