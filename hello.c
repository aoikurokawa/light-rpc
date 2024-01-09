#include <stdio.h>
#include <string.h>

int my_strlen(char *s) {
  char *p = s;

  while (*p != '\0') {
    p++;
  }

  return p - s;
}

int main(void) {
  int a[] = {11, 22, 33};
  int b[3];

  memcpy(b, a, 3 * sizeof(int));

  for (int i = 0; i < 3; i++) {
    printf("%d\n", b[i]);
  }
}
