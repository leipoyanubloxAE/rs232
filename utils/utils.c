#include "utils.h"

void delay(int ms)
{
#ifdef _WIN32
      Sleep(ms);
#else
      usleep(ms*1000);  
#endif
}

