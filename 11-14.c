#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>


#define MAXLINE 100
int main(int argc, char *argv[])
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  socklen_t servlen;

  if (argc != 3) {
    printf("Usage: a.out <hostname/IPaddress> <service/port>");
    exit(EXIT_FAILURE);
  }


  struct addrinfo hints, *res, *ressave;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  // get peer info
  if ((n = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
    printf("error getaddrinfo %s\t%s\n", argv[1], argv[2]);
    exit(EXIT_FAILURE);
  }

  ressave = res;

  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd >= 0) break;
  } while (res = res->ai_next);

  if (res == NULL) {
    printf("error getaddrinfo %s\t%s\n", argv[1], argv[2]);
  }

  struct sockaddr sa;
  char buff[128] = { 0 };
  sa = *res->ai_addr;
  memcpy(&sa, res->ai_addr, res->ai_addrlen);
  
  freeaddrinfo(ressave);

  void *ret;
  switch (sa.sa_family) {
    case AF_INET:
      servlen = INET_ADDRSTRLEN;
      ret = (void *)inet_ntop(sa.sa_family, &(((struct sockaddr_in *)&sa)->sin_addr), buff, servlen);
      if (!ret) {
        printf("error inet_ntop\n");
        exit(EXIT_FAILURE);
      }
      break;

    case AF_INET6:
      printf("AF_INET6...\n");
      break;
  }
  
  printf("sending to %s\n", buff);
  sendto(sockfd, " ", 2, 0, &sa, servlen);
  n = recvfrom(sockfd, recvline, sizeof(recvline), 0, NULL, NULL); /*blocked*/
  recvline[n] = 0;
  fputs(recvline, stdout);

  return 0;
}
