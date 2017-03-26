ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib/libdynamic lib/libclo lib/libreactor

bin_PROGRAMS = rest_server
rest_server_SOURCES = src/rest_server.c src/setup.c
rest_server_CFLAGS = -std=gnu11 -Ofast -march=native -mtune=native -flto -fuse-linker-plugin
rest_server_LDADD = -lreactor -ldynamic -lclo

MAINTAINERCLEANFILES = aclocal.m4 configure Makefile.in
maintainer-clean-local:; rm -rf autotools m4
