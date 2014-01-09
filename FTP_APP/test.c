#include <stdio.h>
#include "logger.h"
int main()
{
  
  int x = 199,i=0;
  char *s = "Bitter Truth";

  while(i<5)
  {
#ifdef CONFIG_DEBUG
   PRINT_D("Hello World ");
#endif
#ifdef CONFIG_FACTORY
   PRINT_F("Hello World SHANMUGAM\n");
#endif
#ifdef CONFIG_DEBUG
   PRINT_D("Zing is back !!! %s %d",s,x++);
#endif
   i++;
  }
}


