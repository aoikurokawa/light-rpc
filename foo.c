#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#include "bar.h"

int main(int argc, char **argv) {
  int x = 3490;
  int *const p = &x;
  int *const *q = &p;

  printf("%d\n", *p + 1);
}
