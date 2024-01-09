#include <stdio.h>

int my_strlen(char *s) {
  char *p = s;

  while (*p != '\0') {
    p++;
  }

  return p - s;
}

int main(void) { printf("%d\n", my_strlen("Hello, world!")); }
