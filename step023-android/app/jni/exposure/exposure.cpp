
#include "Halide.h"

namespace {

using namespace Halide;
using namespace Halide::ConciseCasts;

/**
 * Exposure Metric
 *
 */
class Exposure : public Halide::Generator<Exposure> {

public:
    GeneratorParam<float> up{"up", 0.8f, 0.0f, 1.0f};
    GeneratorParam<float> down{"down", 0.2f, 0.0f, 1.0f};

    Input<Buffer<uint8_t>> input{"input", 2};
    Output<float> lim_down{"lim_down"};
    Output<float> lim_up{"lim_up"};

    void
    generate() {

        Expr h = input.height();
        Expr w = input.width();

        Func hist_rows("hist_rows");
        hist_rows(x, y) = 0;
        RDom rx(0, w);
        Expr bin = cast<int>(clamp(input(rx, y), 0, 255));
        hist_rows(bin, y) += 1;

        Func hist("hist");
        hist(x) = 0;
        RDom ry(0, h);
        hist(x) += hist_rows(x, ry);

        Func p("prob");
        p(x) = f32(hist(x)) / (w * h);

        Func cdf("cdf");
        cdf(x) = p(0);
        RDom b(1, 255);
        cdf(b.x) = cdf(b.x - 1) + p(b.x);

        lim_down() = cdf(u8(255.0f * down));
        lim_up() = 1.0f - cdf(u8(255.0f * up));

        input.dim(0).set_estimate(0, 4000);
        input.dim(1).set_estimate(0, 3000);

        if (!auto_schedule) {
            const int vec = natural_vector_size<uint8_t>();

            hist_rows.in()
                .compute_root()
                .vectorize(x, vec)
                .parallel(y, 4);
            hist_rows.compute_at(hist_rows.in(), y)
                .vectorize(x, vec)
                .update()
                .reorder(y, rx)
                .unroll(y);
            hist.compute_root()
                .vectorize(x, vec)
                .update()
                .reorder(x, ry)
                .vectorize(x, vec)
                .unroll(x, 4)
                .parallel(x)
                .reorder(ry, x);

            p.compute_root();
            cdf.compute_root();

            lim_down.compute_root();
            lim_up.compute_root();
        }
    }

private:
    Var x, y, k;
};

}  // namespace

HALIDE_REGISTER_GENERATOR(Exposure, exposure);