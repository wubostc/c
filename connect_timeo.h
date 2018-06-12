#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void connect_alarm(int);

int connect_timeo(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec) {
  int n;

  struct sigaction act, oact;
  
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = connect_alarm;

  sigaction(SIGALRM, &act, &oact);

  if (alarm(nsec) != 0) {
    printf("connect_timeo: alarm was already set.\n");
  }

  if ((n = connect(sockfd, saptr, salen)) < 0) {
    printf("connect error\n");
    close(sockfd);

    if (errno == EINTR) {
      errno = ETIMEDOUT;
    }
  }

  sigaction(SIGALRM, &oact, NULL);

  alarm(0); // trun off the alarm by pass 0
}

#include <syslog.h>
static void connect_alarm(int signo) {
  syslog(LOG_ERR, "connect_alarm signo: %d", signo);
  return; // JUST INTERRUPT THE CONNECT()
}
