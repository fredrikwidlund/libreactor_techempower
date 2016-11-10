#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <netdb.h>
#include <err.h>

#include <dynamic.h>
#include <clo.h>
#include <reactor.h>

static int realtime_scheduler(void)
{
  struct sched_param param;
  struct rlimit rlim;
  int e;

  e = sched_getparam(0, &param);
  if (e == -1)
    return -1;

  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  rlim = (struct rlimit) {.rlim_cur = param.sched_priority, .rlim_max = param.sched_priority};
  e = prlimit(0, RLIMIT_RTPRIO, &rlim, NULL);
  if (e == -1)
    return -1;

  e = sched_setscheduler(0, SCHED_FIFO, &param);
  if (e == -1)
    return -1;

  return 0;
}

static void plaintext(void *state, reactor_rest_request *request)
{
  (void) state;
  reactor_rest_respond_text(request, "Hello, World!");
}

static void json(void *state, reactor_rest_request *request)
{
  char buffer[4096];
  int e;

  (void) state;
  e = clo_encode((clo[]) {clo_object({"message", clo_string("Hello, World!")})}, buffer, sizeof(buffer));
  if (e == 0)
    reactor_rest_respond_body(request, 200, "OK", "application/json", buffer, strlen(buffer));
  else
    reactor_rest_respond_empty(request, 500, "Internal Server Error");
}

void server(void)
{
  reactor_rest rest;
  int e;

  (void) realtime_scheduler();
  reactor_core_open();
  reactor_rest_init(&rest, NULL, NULL);
  reactor_rest_name(&rest, "*");
  reactor_rest_open(&rest, "localhost", "8080", 0);
  reactor_rest_add_match(&rest, "GET", "/plaintext", plaintext, NULL);
  reactor_rest_add_match(&rest, "GET", "/json", json, NULL);
  e = reactor_core_run();
  if (e == -1)
    err(1, "reactor_core_run");
  reactor_core_close();
}

int main()
{
  long i, n;
  pid_t cpid;

  (void) realtime_scheduler();
  signal(SIGPIPE, SIG_IGN);
  n = MAX(1, sysconf(_SC_NPROCESSORS_ONLN) / 2);
  for (i = 0; i < n; i ++)
    {
      cpid = fork();
      if (cpid == -1)
        err(1, "fork");
      if (cpid == 0)
        {
          server();
          exit(0);
        }
    }
  wait(NULL);
}
