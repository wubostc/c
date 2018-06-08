#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int udp_server(const char *host, const char *serv, socklen_t *addrlenp) {
  int n, sockfd;

  struct addrinfo hints, *res, *ressave;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE; // 被动
  hints.ai_socktype = SOCK_DGRAM;

  if ((n = getaddrinfo(host, serv, &hints, &res) != 0)) {
    printf("error udp_server %s %s\n", host, serv);
    exit(EXIT_FAILURE);
  }

  ressave = res;

  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) continue;
    
    int n = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0) break;

    close(sockfd); // close if bind failure...
  } while (res = res->ai_next);

  if (res == NULL) {
    printf("error udp_server %s %s\n", host, serv);
  }

  *addrlenp = res->ai_addrlen;

  freeaddrinfo(ressave);

  return sockfd;
}
