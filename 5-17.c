// c/s 格式化文本
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in servaddr, chiaddr;
  int servfd, connfd;

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(9999);


  servfd = socket(AF_INET, SOCK_STREAM, 0);

  bind(servfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

  listen(servfd, 10);


  while(1) {
    socklen_t len = sizeof(chiaddr);
    connfd = accept(servfd, (struct sockaddr *)&chiaddr, &len);

    char recvline[10] = { 0 };
    char sendline[10] = { 0 };
    int n;
    int a,b;
    if ((n = read(connfd, recvline, 10) > 0)) {

      if (sscanf(recvline, "%d%d", &a,&b) == 2) {
        snprintf(sendline, 10, "sum: %d", a+b);
      } else {
        snprintf(sendline, 10, "sum: err");
      }
      
      write(connfd, sendline, strlen(sendline));
      close(connfd);
    }
    
  }

  return 0;
}
