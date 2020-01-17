#!/bin/bash

if [ -e Makefile ]
then
    make distclean
fi

rm -rf  aclocal.m4  autom4te.cache  configure m4 Makefile.in
rm -rf src/config.h.in
rm -rf src/Makefil.in
rm -rf src/example/Makefile.in
rm -rf src/mylib4c/Makefile.in
