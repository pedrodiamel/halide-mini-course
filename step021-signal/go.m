
% Add the path to the halide tools.
addpath(fullfile(getenv('HALIDE_PATH'), '.', 'tools'));

% Build the halide pipeline into a mex library.
mex_halide('mfcc.cpp', '-g', 'mfcc');
