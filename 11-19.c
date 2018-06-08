#include "udp_server.h"
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAXLINE 100
int main(int argc, char *argv[])
{
  int sockfd;
  ssize_t n;
  char buff[MAXLINE];
  socklen_t len;
  struct sockaddr_storage cliaddr; 

  printf("Usage: \n");
  printf("\t %s <host/ipaddress> <port/service>: \n", argv[0]);
  puts("or");
  printf("\t %s <port/service>: \n", argv[0]);

  if (argc == 2) {
    sockfd =  udp_server(NULL, argv[1], &len);
  } else if (argc == 3) {
    sockfd = udp_server(argv[1], argv[2], &len);
  } else {
    exit(EXIT_FAILURE);
  }


  socklen_t clilen;
  time_t ticks;
  while (1) {
    clilen = sizeof(cliaddr);
    printf("wait...\n"); 
    recvfrom(sockfd, buff, MAXLINE, 0, (struct sockaddr *)&cliaddr, &clilen);
    printf("client ip %s\n", 
        inet_ntop(
          cliaddr.ss_family, 
          &(((struct sockaddr_in *)&cliaddr)->sin_addr), 
          buff,
          clilen));
    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    sendto(sockfd, buff, strlen(buff), 0, (const struct sockaddr *)&cliaddr, clilen);
  }

  return 0;
}
