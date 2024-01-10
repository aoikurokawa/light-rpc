#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#include "bar.h"

int main(int argc, char **argv) {
  int x = 3490;
  int *p = &x;
  int **q = &p;

  printf("%p %p\n", p, q);
}
