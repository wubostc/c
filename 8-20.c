#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

static int count = 0;

void recvfrom_int(int){
  printf("\ncount: %d\n", count);
  exit(0);
}



int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr, chiaddr;
  int servfd;
  servfd = socket(AF_INET, SOCK_DGRAM, 0);
  
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);

  int recbuf = 220 * 1024; // byte
  setsockopt(servfd, SOL_SOCKET, SO_RCVBUF, &recbuf, sizeof(recbuf));

  if (bind(servfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    puts("bind err\n");
    exit(0);
  }

  int n;
  socklen_t len;

  // SIGINT: ctrl + c
  // INT: interrupt
  signal(SIGINT, recvfrom_int);

  char msg[1400];
  
  while(1) {
    len = sizeof(chiaddr);
    
    n = recvfrom(servfd, msg, 1400, 0, (struct sockaddr *)&chiaddr, &len);
    msg[n] = 0;
    ++count; 
  }


  return 0;
}
