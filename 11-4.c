#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 10
int main(int argc, char *argv[])
{
  int    sockfd, n;
  char   recvline[MAXLINE + 1];
  struct sockaddr_in servaddr;
  struct in_addr **pptr;
  // struct in_addr  *inetaddrp[2];
  struct hostent *hp;
  struct servent *sp;

  if (argc != 3) {
    printf("Usage: $ ./a.out <hostname> <service>\n");
    exit(0);
  }

  if ( (hp = gethostbyname(argv[1])) == NULL ) {
    printf("error gethostbyname: %s\n", hstrerror(h_errno));
  }

  pptr = (struct in_addr **)hp->h_addr_list;


  // getservbyname 
  // read service entry from /etc/service
  if ( (sp = getservbyname(argv[2], "tcp")) == NULL) {
    printf("error getservbyname: %s\n", argv[2]); 
  }

  printf("sp:\n");
  printf("\tname: %s\n", sp->s_name);
  printf("\tport: %d\n", ntohs(sp->s_port));
  char** it = sp->s_aliases;
  for (; *it != NULL; ++it) {
    printf("\talias: %s\n", *it);
  }
  printf("\tproto: %s\n", sp->s_proto);


  for (; *pptr != NULL; ++pptr) {
    puts("hp:");
    char addr[INET_ADDRSTRLEN];
    printf("\taddr: %s\n", inet_ntop(AF_INET, *pptr, addr, INET_ADDRSTRLEN)); 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = sp->s_port; // htons(13)
    servaddr.sin_addr.s_addr = *(in_addr_t*)*pptr;

    puts("servaddr:");
    printf("\taddr: %s\n", inet_ntop(AF_INET, &servaddr.sin_addr.s_addr, addr, INET_ADDRSTRLEN));
  
    if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
      break; // success
    }

    perror("error connecr");
    close(sockfd);

  }

  while ( (n = read(sockfd, recvline, MAXLINE)) > 0 ) {
    recvline[n] = 0;
    fputs(recvline, stdout);
  }


  return 0;
}
