#include <syslog.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
int daemon_init(const char *pname, int facility) {
  int i;
  pid_t pid;

  if ((pid = fork()) < 0) {
    return -1;
  } else if (pid) {
    exit(0); // parent term
  }

  if (setsid() < 0) {
    return -1;
  }

  signal(SIGHUP, SIG_IGN);

  if ((pid = fork()) < 0) {
    return -1;
  } else if (pid) {
    exit(0); // child 1 term
  }

  chdir("/");

#define FDMAX 94526  // cat /proc/sys/fs/file-max
  for (i = 0; i < FDMAX; ++i) {
    /**
     * close stdin stdout stderr
     */
    close(i);
  }

  openlog(pname, LOG_PID, facility);
}
