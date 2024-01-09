#include <stdio.h>
#include <string.h>

struct animal {
  char *name;
  int leg_count;
};

int compar(const void *elem1, const void *elem2) {
  const struct animal *animal1 = elem1;
  const struct animal *animal2 = elem2;

  if (animal1->leg_count > animal2->leg_count) {
    return 1;
  }

  if (animal1->leg_count < animal2->leg_count) {
    return -1;
  }

  return 0;
}

int main(void) {
  char a = 'X';

  void *p = &a;
  char *q = p;

  printf("%c\n", a);
  printf("%c\n", *q);
}

void *my_memcpy(void *dest, void *src, int byte_count) {
  char *s = src, *d = dest;

  while (byte_count--) {
    *d++ = *s++;
  }

  return dest;
}
