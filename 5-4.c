#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&servaddr, 0, sizeof(servaddr));

  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);

  connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

  char sendline[10], recvline[10];
  while(fgets(sendline, 10, stdin) != NULL) {
    write(sockfd, sendline, strlen(sendline));

    read(sockfd, recvline, 10);

    fputs(recvline, stdout);
  }

  _exit(0);

  return 0;
}
