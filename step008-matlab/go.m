
% Add the path to the halide tools.
addpath(fullfile(getenv('HALIDE_PATH'), '.', 'tools'));

% Build the halide pipeline into a mex library.
mex_halide('edge.cpp', '-g', 'edge');
