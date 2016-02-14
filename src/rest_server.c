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
#include <linux/sched.h>
#include <netdb.h>
#include <err.h>

#include <dynamic.h>
#include <clo.h>
#include <reactor_core.h>
#include <reactor_net.h>

struct thread_info
{
  pthread_t tid;
  int       cpu;
};

typedef struct map map;
struct map
{
  void (*handler)(reactor_rest_server_request *, void *);
  void  *state;
};

void json(reactor_rest_server_request *request, void *message)
{
  reactor_rest_server_respond_clo(request, 200, (clo[]) {clo_object({"message", clo_string(message)})});
}

void plaintext(reactor_rest_server_request *request, void *message)
{
  reactor_rest_server_respond_text(request, message);
}

void event(void *state, int type, void *data)
{
  reactor_rest_server_request *request = data;
  map *map = request->state;

  (void) state;
  if (type == REACTOR_REST_SERVER_ERROR)
    warn("error event");
  else if (type == REACTOR_REST_SERVER_REQUEST)
    map->handler(request, map->state);
}

/* don't use in real production environments */
int insane_optimizations(struct thread_info *tinfo)
{
  struct sched_param param;
  struct rlimit rlim;
  cpu_set_t cpuset;
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

  CPU_ZERO(&cpuset);
  CPU_SET(tinfo->cpu, &cpuset);
  e = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  if (e != 0)
    return -1;

  return 0;
}

void *server(void *arg)
{
  reactor_rest_server rest;
  const char message[] = "Hello, World!";

  (void) insane_optimizations(arg);

  reactor_core_construct();
  reactor_rest_server_init(&rest, event, &rest);
  reactor_rest_server_open(&rest, NULL, "8080", (reactor_rest_server_options[]) {{.http_server_options.name = "*"}});
  reactor_rest_server_add(&rest, "GET", "/plaintext", (map[]) {{.handler = plaintext, .state = (void *) message}});
  reactor_rest_server_add(&rest, "GET", "/json", (map[]) {{.handler = json, .state = (void *) message}});
  reactor_core_run();
  reactor_core_destruct();

  return NULL;
}

int main()
{
  long i, n = sysconf(_SC_NPROCESSORS_ONLN);
  struct thread_info tinfo[n];

  signal(SIGPIPE, SIG_IGN);

  for (i = 0; i < n; i ++)
    {
      tinfo[i].cpu = i;
      pthread_create(&tinfo[i].tid, NULL, server, &tinfo[i]);
    }
  for (i = 0; i < n; i ++)
    pthread_join(tinfo[i].tid, NULL);
}
