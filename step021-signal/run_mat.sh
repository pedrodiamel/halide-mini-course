#!/bin/bash

rm -f *.mex *.mexa64
/usr/local/MATLAB/R2018a/bin/matlab -nojvm -nodisplay -r "go; exit"