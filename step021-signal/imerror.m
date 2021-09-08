function e=imerror( a, b)
e = sum( (((double(a(:)) - double(b(:))).^2.0).^0.5) )/numel(a);
end
