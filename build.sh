#!/bin/bash

autoreconf -vif

if [ "$1" = "stderr" ]
then
   ./configure  --enable-stderr
else
   ./configure
fi

make

