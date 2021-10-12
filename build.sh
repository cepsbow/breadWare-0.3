#!/usr/bin/bash
# Builds a clean ELF and UF2 from the source
bWdir=~/pico/breadware
pisdk=~/pico/pico-sdk

export PICO_SDK=$pisdk
cd $bWdir
rm -rf build 
mkdir build 
cd build 
cmake .. 
make MT
