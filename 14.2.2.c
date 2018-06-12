#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include "connect_timeo.h"

void myalarmhdr(int signo) {
  printf("myalarmhdr %d\n", signo);
  return;
}

int main(int argc, char *argv[])
{
  int sockfd; // client
  struct addrinfo hints, *res, *ressave;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
   
  // server addr is 127.0.0.1:9999
  if (getaddrinfo("127.0.0.1", "9999", &hints, &res) != 0) {
    printf("getaddrinfo error\n");
    exit(EXIT_FAILURE);
  }

  ressave = res;

  do {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd > 0) break;

  } while (res = res->ai_next);


  connect_timeo(sockfd, res->ai_addr, res->ai_addrlen, 2);

  struct sigaction act, oact;
  act.sa_flags = 0;
  act.sa_handler = myalarmhdr;
  sigemptyset(&act.sa_mask);
  
  alarm(2);

  sigaction(SIGALRM, &act, &oact);

  freeaddrinfo(ressave);

  printf("writing to server\n");
  write(sockfd, "1", 1);


  printf("reading from server\n");
  char buf[100];
  read(sockfd, &buf, 100);
printf("...");
  return 0;
}
