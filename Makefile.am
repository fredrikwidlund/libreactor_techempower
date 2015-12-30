ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib/libdynamic lib/libreactor_core lib/libreactor_net

bin_PROGRAMS = rest_server
rest_server_SOURCES = src/rest_server.c
rest_server_CFLAGS = -std=gnu11 -O3 -flto -fuse-linker-plugin
rest_server_LDFLAGS = -pthread
rest_server_LDADD = -lreactor_net -lreactor_core -ldynamic

MAINTAINERCLEANFILES = aclocal.m4 configure Makefile.in
maintainer-clean-local:; rm -rf autotools m4
