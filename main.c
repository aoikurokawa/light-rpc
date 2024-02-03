#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct pollfd pfds[1];

  pfds[0].fd = 0;
  pfds[0].events = POLLIN;

  printf("Hit RETURN or wait 2.5 seconds for timeout\n");

  int num_events = poll(pfds, 1, 2500);

  if (num_events == 0) {
    printf("Poll timed out!\n");
  } else {
    int pollin_happened = pfds[0].revents & POLLIN;

    if (pollin_happened) {
      printf("File descriptor %d is ready to read\n", pfds[0].fd);
    } else {
      printf("Unexpected event occured: %d\n", pfds[0].revents);
    }
  }

  return 0;
}
