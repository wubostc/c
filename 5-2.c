#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int listenfd, connfd;
  pid_t childpid;
  socklen_t chilen;

  struct sockaddr_in servaddr, chiaddr;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);


  bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  listen(listenfd, 10);


  while(1) {
    chilen = sizeof(chiaddr);
    connfd = accept(listenfd, (struct sockaddr *)&chiaddr, &chilen);
    if (errno == EINTR) {
      continue;
    }
    
    char ip[17] = { 0 } ;
   printf("client: %s\n", inet_ntop(AF_INET, &chiaddr.sin_addr.s_addr, ip, INET_ADDRSTRLEN)) ;

   // client: ctrl + d

    // fork
    // connfd + 1 ref
    // listenfd + 1 ref
    if((childpid = fork()) == 0) { // child process
      close(listenfd); // listenfd + 1 ref
      char buf[11] = { 0 };
      int n;
      while((n = read(connfd, buf, 10)) > 0) {
        buf[n] = 0;
        printf("%d\n", n);
        write(connfd, buf, strlen(buf) + 1);
      }
      close(connfd);  // connfd -1 ref
      _exit(0);
    }
    
    // parent process
    close(connfd);
  }
  return 0;
}
