#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
  struct addrinfo hints, *res, *ressave;
  int n, sockfd; // servfd

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

  //                port/service
  if (getaddrinfo(NULL, "9999", &hints, &res) != 0) {
    printf("getaddrinfo error\n");
    exit(EXIT_FAILURE);
  }

  ressave = res;

  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
      continue;
    }
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
      close(sockfd);
      continue;
    } else {
      break; // success
    }
  } while (res = res->ai_next);

  freeaddrinfo(ressave);

  if (res == NULL) {
    printf("res is null error\n");
    exit(EXIT_FAILURE);
  }

  listen(sockfd, 10);

  while (1) {
    char buf[100];
    struct sockaddr_storage cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    socklen_t clilen;
    int connfd;
    connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    if (connfd < 0) {
      printf("accept error");
      continue;
    }
    recvfrom(connfd, buf, 100, 0, (struct sockaddr *)&cliaddr, &clilen);
    sleep(10); // sleep for 10 s
    sendto(connfd, "123", 3, 0, (struct sockaddr *)&cliaddr, clilen);
    close(connfd);
  }

  return 0;
}
