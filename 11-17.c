#include <unistd.h>
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
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;
  } while (res = res->ai_next);

  if (res == NULL) {
    printf("error getaddrinfo %s\t%s\n", argv[1], argv[2]);
  }

  
  freeaddrinfo(ressave);

  write(sockfd, "", 0); // 0 bytes
  n = recvfrom(sockfd, recvline, sizeof(recvline), 0, NULL, NULL); /*blocked*/
  recvline[n] = 0;
  fputs(recvline, stdout);

  return 0;
}
