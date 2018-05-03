#include "error.h"

void
check_oom(void* p, char* str)
{
  if(!p)
    {
      printf("%s: out of memory?\n", str); 
      exit(EXIT_FAILURE);
    }
}
