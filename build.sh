#!/bin/sh
set -ex
./clean.sh
mkdir -p ./build/
cc main.c -o ./build/battery_watcher
