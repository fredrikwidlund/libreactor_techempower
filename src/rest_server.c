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

void rest_event(void *state, int type, void *data)
{
  reactor_rest_server_request *request = data;

  (void) state;
  if (type == REACTOR_REST_SERVER_ERROR)
    warn("error event");

  if (type == REACTOR_REST_SERVER_REQUEST)
    reactor_rest_server_respond_text(request, "Hello, World!");
}

int main()
{
  reactor_rest_server rest;

  reactor_core_construct();

  reactor_rest_server_init(&rest, rest_event, &rest);
  reactor_rest_server_open(&rest, NULL, NULL);
  reactor_rest_server_add(&rest, "GET", "/plaintext", "Hello, World!");
  reactor_core_run();
  reactor_core_destruct();
}
