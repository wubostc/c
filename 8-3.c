#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr, chiaddr;
  int servfd;
  servfd = socket(AF_INET, SOCK_DGRAM, 0);
  
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);

  if (bind(servfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    puts("bind err\n");
    exit(0);
  }

  int n;
  socklen_t len;
  char msg[10];
int count = 0;
  while(1) {
    puts("...\n");
    len = sizeof(chiaddr);
    
    n = recvfrom(servfd, msg, 10, 0, (struct sockaddr *)&chiaddr, &len);
    msg[n] = 0;
    printf("len: %d\n", n);
    printf("recv: %s\n", msg);

    // snprintf(msg, 10, "ser:%s", msg);
    
    sendto(servfd, msg, strlen(msg), 0, (const struct sockaddr *)&chiaddr, len);

    printf("count: %d\n", ++count);
  }


  return 0;
}
