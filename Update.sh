#!/bin/bash
cd ~/WarmLoader || exit
git pull
mkdir -p build && cd build
cmake ..
make
sudo cp warmloader /usr/local/bin/
echo "WarmLoader updated!"
