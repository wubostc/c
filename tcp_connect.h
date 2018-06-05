#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int tcp_connect(const char *host, const char *serv) {
  int sockfd, n;
  struct addrinfo hints, *res, *ressave;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0 ) {
    printf("error tcp_connect");
    exit(0);
  }

  ressave = res;

  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    
    if (sockfd < 0) continue;

    if (connect(sockfd, res->ai_addr, res->ai_addrlen)) break;

    close(sockfd);

  } while (res = res->ai_next);

  if (res == NULL) {
    printf("error tcp_connect %s, %s\n", host, serv);
    exit(0);
  }

  freeaddrinfo(ressave);

  return sockfd;
}
