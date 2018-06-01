#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // htonl
#include <string.h>
#include <stdio.h> // snprintf
#include <unistd.h> // wirte


int main(int argc, char *argv[])
{
  int f;
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  char buff[100];
  time_t ticks;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  f = bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  if (f != 0) {
    printf("bind err");
  }

  listen(listenfd, 10);

  while(1) {
    connfd = accept(listenfd, NULL, NULL);
    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%s\r\n", ctime(&ticks));
    write(connfd, buff, strlen(buff));

    close(connfd);
  }
  

  return 0;
}
