#include <stdio.h>

int my_strlen(char *s) {
  char *p = s;

  while (*p != '\0') {
    p++;
  }

  return p - s;
}

int main(void) {
  int a[] = {11, 22, 33, 44, 55};
  int *p = a;

  for (int i = 0; i < 5; i++) {
    printf("%d\n", a[i]);
  }
  for (int i = 0; i < 5; i++) {
    printf("%d\n", p[i]);
  }
  for (int i = 0; i < 5; i++) {
    printf("%d\n", *a + 1);
  }
  for (int i = 0; i < 5; i++) {
    printf("%d\n", *p + 1);
  }
  // printf("%d\n", my_strlen("Hello, world!"));
}
