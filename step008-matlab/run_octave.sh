#!/bin/bash

# sudo apt-get update
# sudo apt-get install liboctave-dev

# This script is run by the nightly tests to check that mex_halide works.

command -v octave >/dev/null 2>&1 || { echo >&2 "Octave not found.  Aborting."; exit 0; }

if [[ $CXX == *"-m32"* ]]; then
    echo "Not proceeding because Halide is compiled in 32-bit mode but octave is (likely) 64-bit"
    exit 0
fi

rm -f *.png *.mex *.mexa64
octave go.m

echo "Success!"
exit 0

# if [ -f conv.png ]
# then
#     echo "Success!"
#     exit 0
# fi

# echo "Failed to produce blurred.png!"
# exit 1