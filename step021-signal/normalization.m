function [ y ] = normalization( x )
y = double(x)/double(max(x(:)));
end

