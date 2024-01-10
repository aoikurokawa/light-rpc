#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#include "bar.h"

int main(int argc, char **argv) {
  for (int i = 10; i >= 0; i--) {
    printf("\rT minus %d second%s...\b", i, i != 1 ? "s" : "");

    fflush(stdout);

    thrd_sleep(&(struct timespec){.tv_sec=1}, NULL);
  }

  printf("\rLiftoff!            \n");
}
