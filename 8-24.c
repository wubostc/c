/**
 * tcp & udp 双重监听
 */
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#define MAXLINE 10

void sig_child(int sig) {
  printf("childsig: %d\n", sig);
  return;
}

int main() {
  int listenfd, connfd, udpfd, nready, maxfdp1;
  char mesg[MAXLINE];
  pid_t childpid;
  fd_set rset; // select
  ssize_t n;
  socklen_t len;
  struct sockaddr_in chiaddr, servaddr;

  int res;

  /* tcp socket */
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(9999); // same as udp
  servaddr.sin_family = AF_INET;
  const int on = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  res = bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  printf("38: %d\n", res);
  listen(listenfd, 100);


  /* udp socket */
  udpfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(9999); // same as tcp
  servaddr.sin_family = AF_INET;
  res = bind(udpfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
  printf("48: %d\n", res);


  signal(SIGCHLD, sig_child);


  FD_ZERO(&rset);
  maxfdp1 = listenfd > udpfd ? listenfd : udpfd;
  maxfdp1 += 1;
  
  while(1){
   
    // every time you set
    FD_SET(listenfd, &rset);
    FD_SET(udpfd, &rset);

    if((nready = select(
            maxfdp1/*int __nfds*/, 
            &rset/*fd_set*__readfds*/, 
            NULL/*fd_set*__writefds*/, 
            NULL/*fd_set*__exceptfds*/, 
            NULL/*structtimeval*__timeout*/)) < 0) {

      // interrupt: ctrl + c
      if (errno == EINTR) {
        continue;
      } else {
        puts("select err.");
        exit(0);
      }

    }

    if(FD_ISSET(listenfd, &rset)) {
      puts("listenfd");
      len = sizeof(chiaddr);
      connfd = accept(listenfd, (struct sockaddr *)&chiaddr, &len);

      if((childpid = fork()) == 0) {
        printf("connfd: %d\n", connfd);
        n = read(connfd, mesg, MAXLINE);
        mesg[n] = 0;
        write(connfd, mesg, strlen(mesg));
        close(connfd);
        close(listenfd); // ref: listenfd - 1
        exit(0); // exits to child process
      }

      close(connfd);
    }

    if(FD_ISSET(udpfd, &rset)) {
      puts("udpfd");
      len = sizeof(chiaddr);
      n = recvfrom(udpfd, mesg, MAXLINE, 0, (struct sockaddr *)&chiaddr, &len);
      mesg[n] = 0;
      sendto(udpfd, mesg, MAXLINE, 0, (struct sockaddr *)&chiaddr, len);
    }

  }

}
