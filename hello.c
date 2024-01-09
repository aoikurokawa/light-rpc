#include <stdio.h>
#include <stdlib.h>
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
  struct animal a[4] = {
      {.name = "Dog", .leg_count = 4},
      {.name = "Monkey", .leg_count = 2},
      {.name = "Antelope", .leg_count = 4},
      {.name = "Snake", .leg_count = 0},
  };

  qsort(a, 4, sizeof(struct animal), compar);

  for (int i = 0; i < 4; i++) {
    printf("%d: %s\n", a[i].leg_count, a[i].name);
  }
}

void *my_memcpy(void *dest, void *src, int byte_count) {
  char *s = src, *d = dest;

  while (byte_count--) {
    *d++ = *s++;
  }

  return dest;
}
