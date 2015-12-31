#!/bin/bash

mkdir -p {.,lib/libdynamic,lib/libreactor_core,lib/libreactor_net}/{m4,autotools}

autoreconf --force --install
