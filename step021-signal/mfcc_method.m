
clc;
clear;

% Speech Processing for Machine Learning: Filter banks, Mel-Frequency 
% Cepstral Coefficients (MFCCs) and What's In-Between
% https://haythamfayek.com/2016/04/21/speech-processing-for-machine-learning.html


%% Setup

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

%% Pre-Emphasis
% $y(t) = x(t) - \alpha*x(t-1)$
 
pre_emph = 0.97;
wave_emph = wave(2:end) - pre_emph .*  wave(1:end-1);
wave_emph = cat(1, wave(1), wave_emph);

figure;
plot(t,wave_emph)
xlabel('Time')
ylabel('Audio Signal')
title('Signal in the Time Domain after Pre-Emphasis')

%% Framing

% Convert from seconds to samples
frame_size = 0.025;
frame_stride = 0.01;
frame_length = frame_size * sr;
frame_step = frame_stride * sr;
signal_length = length(wave_emph);
frame_length = fix(round(frame_length));
frame_step = fix(round(frame_step));

% Make sure that we have at least 1 frame
num_frames = fix( ceil( abs(signal_length - frame_length) / frame_step ) );

% Pad Signal to make sure that all frames have equal number of samples 
% without truncating any samples from the original signal
pad_signal_length = num_frames * frame_step + frame_length;
wave_pad = padarray(wave_emph, [pad_signal_length - signal_length 0], 0, 'post');

index = repmat(1:frame_length, num_frames, 1) + repmat( 1:frame_step:(num_frames*frame_step), frame_length, 1)' - 1;
frames = wave_pad(index);

figure;
imshow(frames, [])
size(frames)

%% Window
% w[n] = 0.54 - 0.46 \cos( 2\pi*n/(N-1)  )
hammingfilter = @(N)( 0.54 - 0.46 * cos( 2*pi*(0:N)/N ));

N = frame_length -1;
w = hammingfilter(N);

frames_ham = frames .* w; 

figure;
% plot(w)
imshow(frames_ham,[])

%% Fourier-Transform and Power Spectrum
% P = | FFT(x_i) |^2 / N 

fftresize = @(x)( x(:,1:(floor(size(x,2)/2)+1)) );

NFFT = 512;
% n = 2^nextpow2(NFFT);
Frames_mag = abs(fftresize(fft(frames_ham, NFFT, 2))); % Magnitude of the FFT
Frames_pow = ( (1.0/NFFT) .* (Frames_mag.^2) );  % Power Spectrum

figure;
imagesc(rot90(Frames_pow,1))


%% Filter Banks

hz2mel = @( hz )( 2595*log10(1+(hz/2)/700) ); % Hertz to mel warping function
mel2hz = @( mel )( 700*(10.^(mel./2595.0)-1)); % mel to Hertz warping function

nfilt = 40;
low_freq_mel = 0;
high_freq_mel = hz2mel(sr);
mp = linspace(low_freq_mel, high_freq_mel, nfilt+2 );
h = mel2hz(mp);
f = floor((NFFT + 1) * h/sr );

H = zeros(nfilt, fix(floor((NFFT/2 + 1))));
for m=1:nfilt
    k = fix(f(m)):fix(f(m+1));
    H(m + k*nfilt) = (k-f(m))./(f(m+1)-f(m)); 
    k = fix(f(m+1)):fix(f(m+2));
    H(m + k*nfilt) = (f(m+2)-k)/(f(m+2)-f(m+1));
end

% figure;
% imagesc(H)

filter_banks = Frames_pow*H';
filter_banks = filter_banks.*(filter_banks ~= 0) + ones('like',filter_banks) .* (filter_banks == 0)*eps;
filter_banks = 20*log10(filter_banks);

figure;
imagesc(filter_banks')
axis('xy')



%% Mel-frequency Cepstral Coefficients (MFCCs)

% Type III DCT matrix routine (see Eq. (5.14) on p.77 of [1])
dctm = @( N, M )( sqrt(2.0/M) * ( 1 ./ sqrt(1+((1:N) == 1))).' .* cos( repmat((0:N-1).',1,M) ...
                                   .* repmat(pi*((1:M)-0.5)/M,N,1) ) );

ceplifter = @( N, L )( 1+0.5*L*sin(pi*(0:N-1)/L) );
                               
num_ceps = 12;
cep_lifter = 22;

%MFCC = dct(filter_banks,[],1,'Type',2);
%MFCC = MFCC(:,2:num_ceps);

HDCT = dctm(num_ceps+1,nfilt);
MFCC = filter_banks * HDCT';
MFCC = MFCC(:,2:num_ceps); % Keep 2-13

[nframes,ncoef] = size(MFCC);
lift = ceplifter(ncoef, cep_lifter);
MFCC = MFCC .* lift;

figure;
imagesc(MFCC')
axis('xy')


%% Mean Normalization

normalize = @(x)(x-(mean(x) - 1e-8));

filter_banks_means = normalize(filter_banks);

figure;
imagesc(filter_banks_means')
axis('xy')

MFCC_means = normalize(MFCC);

figure;
imagesc(MFCC_means')
axis('xy')
