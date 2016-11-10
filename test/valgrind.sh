#!/bin/sh

if command -v valgrind; then
    for file in test/test_*.c
    do
        echo [$file]
        if ! valgrind --error-exitcode=1 --track-fds=yes \
             --read-var-info=yes --leak-check=full --show-leak-kinds=all ${file%.c}
        then
            exit 1
        fi
    done
fi 
