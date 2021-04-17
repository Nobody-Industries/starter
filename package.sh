#!/bin/bash
APPNAME="APP"
if [ "$1" == "b" ]
then
    echo "= BUILD ="
	cc -std=c11 main.c -o $APPNAME -Os -DNDEBUG -g0 -Wall `sdl2-config --cflags` `sdl2-config --libs`
	echo "Size: $(du -skh ./$APPNAME)"
fi
if [ "$1" == "d" ]
then
    echo "= DEBUG ="
	cc -std=c11 main.c -o $APPNAME -DDEBUG -Wall -Wpedantic -Wshadow -Wextra -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=int-conversion -Wvla -g -Og -fsanitize=address -fsanitize=undefined `sdl2-config --cflags` `sdl2-config --libs`
	echo "Size: $(du -skh ./$APPNAME)"
fi
if [ "$1" == "f" ]
then
    echo "= FORMAT ="
    clang-format -i *.c > /dev/null 2> /dev/null
    clang-format -i *.h > /dev/null 2> /dev/null
fi
if [ "$1" == "l" ]
then
    echo "= LEAKS ="
    leaks --atExit -- ./$APPNAME
fi
