#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *x;

  // x = malloc(sizeof(int) * 10);

  if (x == NULL) {
    printf("Error allocating 10 ints\n");
  }

  if ((x = malloc(sizeof(int) * 10)) == NULL) {
    printf("Error allocating 10 ints\n");
  }
}
