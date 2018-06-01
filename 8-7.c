#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int sockfd;

  struct sockaddr_in servaddr;


  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr); 
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(9999);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  int n;
  char sendline[10] = { 0 }, recvline[11] = { 0 };

  while(fgets(sendline, 10, stdin) != NULL) {


    sendto(sockfd, sendline, strlen(sendline), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(sockfd, recvline, 10, 0, NULL, NULL);

    fputs(recvline, stdout);

    // close(sockfd);

    memset(sendline, 0, 10);
    memset(recvline, 0, 11);
  }

  return 0;
}
