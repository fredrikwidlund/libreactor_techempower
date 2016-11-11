AC_INIT([libreactor_techempower], [0.9.0], [fredrik.widlund@gmail.com])
AC_CONFIG_AUX_DIR([autotools])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign no-define subdir-objects])

: ${CFLAGS="-Wall -Wpedantic"}

CFLAGS="$CFLAGS -march=native -mtune=native -fomit-frame-pointer -I$(pwd)/lib/{libdynamic,libclo,libreactor}/src"
LDFLAGS="$LDFLAGS -march=native -mtune=native -fomit-frame-pointer -L$(pwd)/lib/{libdynamic,libclo,libreactor}/.libs"
export CFLAGS LDFLAGS

AC_PROG_CC
AM_PROG_CC_C_O

AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([lib/libdynamic lib/libclo lib/libreactor])
AC_OUTPUT
