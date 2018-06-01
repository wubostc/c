#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int listenfd, connfd;
  socklen_t len;

  struct sockaddr_in servaddr, cliaddr;

  char buff[100];

  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(9999);
  servaddr.sin_family = AF_INET;


  bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  

  listen(listenfd, 10);

  while(1) {
    len = sizeof(cliaddr);

    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);

    printf("connection from %s, port %d\n",
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, 16),
        ntohs(cliaddr.sin_port));

    ticks = time(NULL);
    
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    write(connfd, buff, strlen(buff));

    close(connfd);
  }


  return 0;
}
