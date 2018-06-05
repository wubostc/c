#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 100
int main(int argc, char *argv[])
{
  int listenfd, connfd;
  socklen_t servlen, clilen;
  char buff[MAXLINE];
  struct sockaddr_storage cliaddr;

  time_t ticks;

  if (argc != 2) {
    printf("Usage: $ ./a.out <service or port>\n");
    exit(EXIT_FAILURE);
  }

  
  
  struct addrinfo hints, *res, *ressave; // hints 暗示值
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
 // 被动打开, 将用 bind, 用于服务端
  hints.ai_flags = AI_PASSIVE;
  hints.ai_socktype = SOCK_STREAM;

  int n;
  if ((n = getaddrinfo(
          NULL/*const char*__name*/, // => INADDR_ANY
          argv[1]/*const char*__service*/, 
          &hints/*const struct addrinfo*__req*/, 
          &res/*structaddrinfo**__pai*/) != 0)) {
    printf("getaddrinfo error: %s", argv[1]);
  }
  
  ressave = res;
  do {
    puts("bind...");
    listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (listenfd < 0) continue;
    
    const int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    n = bind(listenfd, res->ai_addr, res->ai_addrlen);

    if (n == 0) break;

    close(listenfd); // bind error ,close

  } while (res = res->ai_next);

  listen(listenfd, 100);

  servlen = res->ai_addrlen;

  freeaddrinfo(ressave);



  while (1) {
    printf("accept...\n");

    clilen = sizeof(clilen); 

    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    
    char buf[128];

    switch (cliaddr.ss_family) {
      case AF_INET:
        {
          printf("AF_INET connection from %s\n", 
              inet_ntop(
                AF_INET, 
                &(((struct sockaddr_in *)&cliaddr)->sin_addr), 
                buf, 
                INET_ADDRSTRLEN));
          printf("port %d\n", ntohs(((struct sockaddr_in *)&cliaddr)->sin_port));
        }
        break;

      case AF_INET6:
        {
          printf("AF_INET6 connection from %s\n",
              inet_ntop(
                AF_INET6, 
                &(((struct sockaddr_in6 *)&cliaddr)->sin6_addr), 
                buf, 
                INET6_ADDRSTRLEN));
        }
        break;

      default:
        break;
    }

    ticks = time(NULL); 
    snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
    write(connfd, buf, strlen(buf));
    close(connfd);
  }

  return 0;
}
