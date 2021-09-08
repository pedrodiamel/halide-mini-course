#include "Halide.h"
#include <stdint.h>
#include <math.h>

namespace
{
    using namespace Halide;
    using namespace Halide::ConciseCasts;

    #ifndef M_PI
    #define M_PI 3.14159265358979310000
    #endif
    const float kPi = static_cast<float>(M_PI);

    /**
     * @brief Mel-Frequency Cepstral Coefficients
     *
     * In sound processing, the mel-frequency cepstrum is a representation of the short-term
     * power spectrum of a sound, based on a linear cosine transform of a log power spectrum
     * on a nonlinear mel scale of frequency.
     *
     * Ref: https://haythamfayek.com/2016/04/21/speech-processing-for-machine-learning.html
     *
     */
    class Mfcc : public Halide::Generator<Mfcc>
    {
    private:
        Var x, y, z;

    public:
        // Inputs
        Input<Buffer<float>> input{"wave", 1};
        Input<float> sample_rate{"sample_rate"};
        Input<float> emphasis{"emphasis"};
        Input<float> frame_size{"frame_size"};
        Input<float> frame_stride{"frame_stride"};

        // Output
        Output<Buffer<float>> output{"mfcc", 2};

        void generate()
        {

            Func wave("wave"), wave_emph("wave_emph");
            wave(z) = input(z);

            // Pre-Emphasis
            // $y(t) = x(t) - \alpha*x(t-1)$
            wave_emph(z) = wave(z + 1) - emphasis * wave(z);

            // Framing
            Expr frame_length = cast<int>(round(frame_size * sample_rate));
            Expr frame_step = cast<int>(round(frame_stride * sample_rate));
            Expr signal_length = input.width();
            Expr num_frames = cast<int>(ceil(abs(signal_length - frame_length) / frame_step));

            RDom r(0, frame_length);

            Func frames("frame");
            Expr x0 = clamp(x, 0, frame_length);
            Expr y0 = clamp(y, 0, num_frames);
            frames(x, y) = wave_emph(x0 + y0 * frame_step);

            // Window
            // w[n] = 0.54 - 0.46 \cos( 2\pi*n/(N-1)  )
            Expr ham = 0.54f - 0.46f * cos( 2.0f*kPi*x/frame_length );
            Func frame_ham("frame_hamming");
            frame_ham(x,y) = frames(x, y)*ham;

            // Continue ...
            // Fourier-Transform and Power Spectrum
            // https://github.com/halide/Halide/tree/master/apps/fft
            // code here ...


            output(x, y) = frame_ham(x, y);

            /* ESTIMATES */
            {
                input.dim(0).set_estimate(0, 16000 * 5);
                output.set_estimate(x, 0, 16000 * 5)
                    .set_estimate(y, 0, 16000);
            }

            // Schedule
            if (auto_schedule)
            {
                // nothing
            }
            else
            {
            }
        }
    };

} // namespace

HALIDE_REGISTER_GENERATOR(Mfcc, mfcc)