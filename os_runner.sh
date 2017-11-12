#!/bin/zsh

if [[ ! -e os/bin ]]; then
  mkdir os/bin
fi
cd os/bin
cmake ../
make clean
make iso
make run
