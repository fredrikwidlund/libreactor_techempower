#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <setjmp.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <err.h>

#include <dynamic.h>
#include <reactor_core.h>
#include <reactor_net.h>

typedef struct map map;
struct map
{
  void (*handler)(reactor_rest_server_request *, void *);
  void  *state;
};

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

void *server(void *arg)
{
  reactor_rest_server rest;
  char message[] = "Hello, World!";

  (void) arg;
  reactor_core_construct();
  reactor_rest_server_init(&rest, event, &rest);
  reactor_rest_server_open(&rest, NULL, "http-alt");
  reactor_rest_server_add(&rest, "GET", "/plaintext", (map[]){{.handler = plaintext, .state = message}});
  reactor_core_run();
  reactor_core_destruct();

  return NULL;
}

int main()
{
  long i, n = sysconf(_SC_NPROCESSORS_ONLN);
  pthread_t tid[n];

  for (i = 0; i < n; i ++)
    pthread_create(&tid[i], NULL, server, NULL);
  for (i = 0; i < n; i ++)
    pthread_join(tid[i], NULL);
}
