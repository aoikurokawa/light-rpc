#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 500

int main(int argc, char *argv[]) {
  int sfd, s;
  char buf[BUF_SIZE];
  ssize_t nread;
  socklen_t peer_addrlen;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  struct sockaddr_storage peer_addr;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  s = getaddrinfo(NULL, argv[1], &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

    if (sfd == -1) {
      continue;
    }

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
      break;
    }

    close(sfd);
  }

  freeaddrinfo(result);

  if (rp == NULL) {
    fprintf(stderr, "Cound not bind\n");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    char host[NI_MAXHOST], service[NI_MAXSERV];

    peer_addrlen = sizeof(peer_addr);
    nread = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&peer_addr,
                     &peer_addrlen);

    if (nread == -1) {
      continue;
    }

    s = getnameinfo((struct sockaddr *)&peer_addr, peer_addrlen, host,
                    NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

    if (s == 0) {
      printf("Received %zd bytes from %s:%s\n", nread, host, service);
    } else {
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    }

    if (sendto(sfd, buf, nread, 0, (struct sockaddr *)&peer_addr,
               peer_addrlen) != nread) {
      fprintf(stderr, "Error sending response\n");
    }
  }
}
