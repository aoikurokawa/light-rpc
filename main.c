#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT "4950"
#define BACKLOG 10
#define MAXBUFLEN 100

void sigchld_handler(int s) {
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;

  errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
  int sockfd, new_fd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char s[INET_ADDRSTRLEN];
  int rv;
  socklen_t addr_len;
  int numbytes;
  char buf[MAXBUFLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "listener: failed to bind");
    exit(1);
  }

  freeaddrinfo(servinfo);

  printf("listener: waiting for recvfrom...\n");

  addr_len = sizeof their_addr;
  if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                           (struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
  }

  printf("listener: got packet from %s\n",
         inet_ntop(their_addr.ss_family,
                   get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
  printf("listner: packet is %d bytes long\n", numbytes);
  buf[numbytes] = '\0';

  printf("listner: packet contains \"%s\"\n", buf);

  close(sockfd);

  return 0;
}
