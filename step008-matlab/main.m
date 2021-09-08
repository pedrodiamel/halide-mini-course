
% /usr/local/MATLAB/R2018a/bin/matlab -nojvm -nodisplay -r "main; exit"
% octave main.m

% Load an input image.
in = cast(imread('../rec/gray.png'), 'uint8');
out = zeros(size(in), 'uint8');

tic;
image = zeros(size(in)+2, 'uint16');
[n,m] = size(image);
x=2:n-1; y=2:m-1;
image(x, y) = cast(in, 'uint16');
gx = (image(x+1, y)-image(x-1, y))./2;
gy = (image(x, y+1)-image(x, y-1))./2;
g_mag = gx.*gx + gy.*gy;
out = cast(g_mag, 'uint8');
toc;

% disp( size(out) )
disp( min(out(:) ) )
disp( max(out(:) ) )

out_halide = zeros(size(in), 'uint8');

tic;
edge(in, out_halide);
toc;

% disp( size(out_halide) )
disp( min(out_halide(:) ) )
disp( max(out_halide(:) ) )

mkdir bin
imwrite( cast(out_halide, 'uint8'), 'bin/edge.png');
imwrite( cast(out, 'uint8'), 'bin/ref_edge.png');
max(max(max(abs(out - out_halide))))
