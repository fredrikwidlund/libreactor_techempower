#!/bin/bash

mkdir -p {.,lib/libdynamic,lib/libclo,lib/libreactor}/{m4,autotools}

autoreconf --force --install
