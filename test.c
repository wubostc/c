#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig_num){
  printf("catch signal: %d\n", sig_num);
  return;
}

int main( void )
{
  typedef void (*sig_handler)(int);

  sig_handler pf = handler;

  signal(SIGINT, pf);

  puts("<C-\\> to exit...\n");

  while(1);
}
