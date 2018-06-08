#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "daemon_init.h"
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

int tcp_listen(char const *host, char const *service, socklen_t *addrlenp) {

  struct addrinfo hints, *res, *ressave;
  int sockfd;

  memset(&hints, 0, sizeof(hints)); 
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(host, service, &hints, &res) != 0) {
    syslog(LOG_ERR, "tcp_listen getaddrinfo"); // deamon process
    exit(-1);
  }

  ressave = res;

  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) continue;
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
      syslog(LOG_ERR, "tcp_listen bind"); // use syslog while running on deamon process now
      close(sockfd); // close fd if bind failure...
    } else {
      break; // success
    }
  } while (res = res->ai_next);

  if (res->ai_addr == NULL) {
    syslog(LOG_ERR, "tcp_listen res->ai_addr is NULL"); // deamon process
    exit(-1);
  }
  
  listen(sockfd, 100);

  if (addrlenp)
    *addrlenp = res->ai_addrlen;

  freeaddrinfo(ressave);

  return sockfd;
}

int main(int argc, char *argv[])
{
  int listenfd, connfd;
  socklen_t servlen, clilen;
  struct sockaddr cliaddr;

  if (argc < 2 || argc > 3) {
    printf("Usage: %s [ <host> ] <service/port>", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (argc == 2) {
    listenfd = tcp_listen(NULL, argv[1], &servlen);
  } else {
    listenfd = tcp_listen(argv[1], argv[2], &servlen);
  }

  while (1) {
    char buf[100];
    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, &cliaddr, &clilen);

    switch (cliaddr.sa_family) {
      case AF_INET:
        {
          const char * ret =
          inet_ntop(
              cliaddr.sa_family, 
              &(((struct sockaddr_in *)&cliaddr)->sin_addr), 
              buf, INET_ADDRSTRLEN);

          if (!ret) {
            syslog(LOG_ERR, "inet_ntop %d", cliaddr.sa_family);
          } else {
            syslog(LOG_USER | LOG_INFO, 
                "accept ip from %s:%d", 
                buf, ntohs(((struct sockaddr_in *)&cliaddr)->sin_port));
          }
        }
        break;

      case AF_INET6:
        {
          // NOTHING...
        }
        break;
    }

    time_t ticks;
    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s", ctime(&ticks));
    write(connfd, buf, strlen(buf));
    
    close(connfd);
  }

  return 0;
}
