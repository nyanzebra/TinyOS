#!/bin/zsh

if [[ -e os/bin ]]; then
  rm -rf os/bin
fi
mkdir os/bin
cd os/bin
cmake ../
make clean
make iso
make run
