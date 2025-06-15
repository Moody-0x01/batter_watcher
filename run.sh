#!/bin/sh
set -ex
./build.sh
./kill.sh
./build/battery_watcher & disown
echo "Started"
