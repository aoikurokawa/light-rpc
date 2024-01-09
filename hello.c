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
  char s[] = "Goats!";
  char t[100];

  memcpy(t, s, 7);

  printf("%s\n", t);
}
