#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline(FILE *fp) {
  int offset = 0;
  int bufsize = 4;
  char *buf;
  int c;

  buf = malloc(bufsize);

  if (buf == NULL) {
    return NULL;
  }

  while (c = fgetc(fp), c != '\n' && c != EOF) {
    if (offset == bufsize - 1) {
      bufsize *= 2;

      char *new_buf = realloc(buf, bufsize);

      if (new_buf == NULL) {
        free(buf);
        return NULL;
      }

      buf = new_buf;
    }

    buf[offset++] = c;
  }

  if (c == EOF && offset == 0) {
    free(buf);
    return NULL;
  }

  if (offset < bufsize - 1) {
    char *new_buf = realloc(buf, offset + 1);

    if (new_buf != NULL) {
      buf = new_buf;
    }
  }

  buf[offset] = '\0';

  return buf;
}

int main(void) {
  FILE *fp = fopen("whale.txt", "r");

  char *line;

  while ((line = readline(fp)) != NULL) {
    printf("%s\n", line);
    free(line);
  }

  fclose(fp);
}
