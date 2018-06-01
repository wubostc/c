#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton
#include <string.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
  int sockfd, n;
  char recvline[8000];
  struct sockaddr_in servaddr;

  if (argc != 2) {
    std::cout << "Usage: $ ./a.out 192.168.1.202\n";
    exit(0);
  }

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    std::cout << "socket\n";
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // inet_pton 点分十进制 转 大端二进制
  if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
    std::cout << ("inet_pton error\n");
    exit(0);
  } 

  // 初始化socket
  if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    std::cout << ("connect error\n");
    exit(0);
  }
  
  while( (n = read(sockfd, recvline, 8000)) > 0 ) {
    std::cout << n << std::endl;
    recvline[n] = 0; // null term
  }

    std::cout << recvline;
  if (n < 0) {
    std::cout << "read error\n";
  }
  
  
  return 0;
}
