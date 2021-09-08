#!/bin/bash

echo 'export HALIDE_PATH="/opt/Halide"' >> ~/.bashrc
echo 'export PATH="$HALIDE_PATH/bin:$PATH"' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH="$HALIDE_PATH/bin:$LD_LIBRARY_PATH"' >> ~/.bashrc
echo 'export DYLD_LIBRARY_PATH="$HALIDE_PATH/bin:$DYLD_LIBRARY_PATH"' >> ~/.bashrc
echo 'export HL_INCLUDE="$HALIDE_PATH/include"' >> ~/.bashrc
echo 'export HL_LIBRARY="$HALIDE_PATH/bin"' >> ~/.bashrc

source ~/.bashrc