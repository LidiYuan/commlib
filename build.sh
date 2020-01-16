#!/bin/bash

rm -rf m4

autoreconf -vif

./configure

make

