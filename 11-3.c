#include <arpa/inet.h>
#include <netdb.h> // struct hostent;  host entry
#include <stdio.h>

/**
 * Usage: 
 * $ ./a.out www.baidu.com sina.com
 */
int main(int argc, char *argv[])
{
  char *ptr, **pptr;  
  char str[INET_ADDRSTRLEN];
  struct hostent *hptr;

  while(--argc > 0) {
    ptr = *++argv;

    if ( (hptr = gethostbyname(ptr)) == NULL ) {
      printf("host: %s : %s\n", ptr, hstrerror(h_errno));
      continue;
    }

    printf("official hostname: %s\n", hptr->h_name);

    for (pptr = hptr->h_aliases; *pptr != NULL; ++pptr) {
      printf("\talias: %s\n", *pptr);
    }

    switch (hptr->h_addrtype) {
      case AF_INET:
        {
          pptr = hptr->h_addr_list;
          for (; *pptr != NULL; ++pptr) {
            printf("\taddress: %s\n", inet_ntop(AF_INET, *pptr, str, INET_ADDRSTRLEN));
          }
        }
        break;
      
      default:
        break;
    }

  }
  return 0;
}
