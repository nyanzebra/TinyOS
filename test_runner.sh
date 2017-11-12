#!/bin/zsh

if [[ -e os/tst/bin ]]; then
  rm -rf os/tst/bin
fi
mkdir os/tst/bin
cd os/tst/bin
cmake ..
make clean
make testos
./testos;
