clc;
clear;

% Speech Processing for Machine Learning: Filter banks, Mel-Frequency
% Halide implementation test

%%

[wave, sr] = audioread('../rec/OSR_us_000_0010_8k.wav', 'native');
wave = wave(1:fix(3.5*sr));
wave = double(wave);
t = 0:1/sr:length(wave)/sr;
t = t(1:end-1);

figure;
plot(t,wave)
xlabel('Time')
ylabel('Audio Signal')
title('Signal in the Time Domain')

%%

emphasis = 0.97;
frame_size = 0.025;
frame_stride = 0.01;
frame_length = frame_size * sr;
frame_step = frame_stride * sr;
signal_length = length(wave);
frame_length = fix(round(frame_length));
frame_step = fix(round(frame_step));
num_frames = fix( ceil( abs(signal_length - frame_length) / frame_step ) );


wave_in = cast(wave, 'single');
sample_rate_in = single(sr);
emphasis_in = single(emphasis);
frame_size_in = single(frame_size);
frame_stride_in = single(frame_stride);
out = zeros([frame_length num_frames], 'single');


tic;
mfcc(wave_in, sample_rate_in, emphasis_in, frame_size_in, frame_stride_in, out);
toc;

tic;
out_ref = mmfcc(wave_in, sr, emphasis, frame_size, frame_stride);
toc;

size(out)
size(out_ref)

figure;
imagesc( normalization(out));
title('halide');

figure;
imagesc(normalization(out_ref'));
% axis('xy')
title('matlab');


figure;
imagesc((normalization(out_ref') - normalization(out)))
title('error');
imerror(out_ref', out)


