#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <setjmp.h>
#include <cmocka.h>
#include <errno.h>
#include <string.h>
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

void rest_event(void *state, int type, void *data)
{
  reactor_rest_server_request *request = data;
  map *map = request->state;

  (void) state;
  if (type == REACTOR_REST_SERVER_ERROR)
    warn("error event");
  else if (type == REACTOR_REST_SERVER_REQUEST)
    map->handler(request, map->state);
}

int main()
{
  reactor_rest_server rest;
  char message[] = "Hello, World!";

  reactor_core_construct();

  reactor_rest_server_init(&rest, rest_event, &rest);
  reactor_rest_server_open(&rest, NULL, NULL);
  reactor_rest_server_add(&rest, "GET", "/plaintext", (map[]){{.handler = plaintext, .state = message}});
  reactor_core_run();
  reactor_core_destruct();
}
