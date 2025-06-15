# Dependencies
- Only [dunst](https://github.com/dunst-project/dunst "Lightweight and customizable notification daemon")

# Set up + Usage
- Install dunst with pacman, dnf, or apt-get, then get it started with this command `dunst & disown`
- run `./run.sh` which will compile the binary and run it as an independent process
- build `./build.sh` to just build the binary and place it in the `./build` folder.
- kill `./kill.sh` kills the watcher.
