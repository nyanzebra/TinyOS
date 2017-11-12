#!/bin/zsh

cmake -Bos/tst/bin -Hos/tst
cd os/tst/bin
make clean
make testos
./testos;
