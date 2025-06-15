#!/bin/sh
set -ex
./build.sh
./build/battery_watcher & disown
echo "Started"
