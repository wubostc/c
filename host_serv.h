#include <netdb.h>
#include <string.h>

struct addrinfo *host_serv(
    const char *host, 
    const char *service,
    int family, 
    int socktype) {

  int n;
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_CANONNAME; // canonical name
  hints.ai_family = family; // AF_UNSPEC, AF_INET, AF_INET6
  hints.ai_socktype = socktype; // 0, SOCK_STREAM, SOCK_DGRAM

  if ( (n = getaddrinfo(host, service, &hints, &res) != 0) ) {
    return NULL;
  }

  return res;
}
