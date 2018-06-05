#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#define MAXLINE 100
int main(int argc, char *argv[])
{
  int sockfd, n;
  char recvline[MAXLINE + 1];
  socklen_t len;
  struct sockaddr_storage ss;

  if (argc != 3) {
    // $ ./a.out baidu.com www
    // $ ./a.out time.nist.gov daytime
    printf("Usage: $ ./a.out <hostname/IPaddress> <service/port>\n");

    exit(0);
  }

  // sockfd = tcp_connect(argv[1], argv[2]);
  
  struct addrinfo hints, *res, *ressave;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  n = getaddrinfo(argv[1], argv[2], &hints, &res);
  
  if (n != 0) {
    printf("error getaddrinfo.\n");
    exit(n);
  }

  ressave = res;

  do {
    puts("...");
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd < 0) continue;

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;

    close(sockfd);

  } while (res = res->ai_next);

  len = sizeof(ss);
  
  char str[128];
  
  // connect -> get peer sockaddr
  getpeername(sockfd, (struct sockaddr *)&ss, &len);

  switch (((struct sockaddr *)&ss)->sa_family) {
    case AF_INET:
      {
        inet_ntop(AF_INET, &(((struct sockaddr_in *)&ss)->sin_addr), str, sizeof(str));
      }
      break;

    case AF_INET6:
      {
        inet_ntop(AF_INET, &(((struct sockaddr_in6 *)&ss)->sin6_addr), str, sizeof(str));
      }
      break;

    default:
      break;
  } 

  printf("connected to %s\n", str);

  // tcp:connect -> read by sockfd
  while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    fputs(recvline, stdout);
  }

  freeaddrinfo(ressave);

  return 0;
}
