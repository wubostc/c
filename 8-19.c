#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  int sockfd;

  struct sockaddr_in servaddr;


  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr); 
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

#define NDG   2000
#define DGLEN 1400
  char sendline[DGLEN];
  
  int i;
  for(i = 0; i < 2000; ++i) {
    sendto(sockfd, sendline, DGLEN, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  }

  return 0;
}
