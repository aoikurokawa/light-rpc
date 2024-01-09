#include <stdio.h>

int main(void) {
  int a[] = {11, 22, 33, 44, 55, 999};
  int *p = &a[0];

  while (*p != 999) {
    printf("%d\n", *p);
    p++;
  }
}
