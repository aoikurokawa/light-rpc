#include "unp.h"
#include <errno.h>
#include <string.h>

static void err_doit(int, int, const char *, va_list);

void err_quit(const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  err_doit(0, LOG_ERR, fmt, ap);
  va_end(ap);
  exit(1);
}

/* Print message and return to caller
 * Caller specifies "errnoflag" and "level" */

static void err_doit(int errnoflag, int level, const char *fmt, va_list ap) {
  int errno_save, n;
  char buf[MAXLINE + 1];

  errno_save = errno; /* value caller might want printed */
#ifdef HAVE_VSNPRINTF
  vsnprintf(buf, MAXLINE, fmt, ap); /* safe */
#else
  vsprintf(buf, fmt, ap); /* not safe */
#endif
  n = strlen(buf);
  if (errnoflag)
    snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
  strcat(buf, "\n");

  if (daemon_proc) {
    syslog(level, buf);
  } else {
    fflush(stdout); /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(stderr);
  }
  return;
}
