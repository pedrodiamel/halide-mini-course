function out = mmfcc(wave, sr, pre_emph, frame_size, frame_stride)

% Pre-Emphasis
wave_emph = wave(2:end) - pre_emph .*  wave(1:end-1);
wave_emph = cat(1, wave(1), wave_emph);

% Framing
% Convert from seconds to samples
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

% Window
% w[n] = 0.54 - 0.46 \cos( 2\pi*n/(N-1)  )
hammingfilter = @(N)( 0.54 - 0.46 * cos( 2*pi*(0:N)/N ));

N = frame_length -1;
w = hammingfilter(N);

frames_ham = frames .* w; 

out = frames_ham;

end

