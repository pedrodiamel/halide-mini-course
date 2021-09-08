#include "Halide.h"
#include "halide_image_io.h"

#include <stdio.h>

using namespace Halide;
using namespace Halide::Tools;

Func h, hnorm, pdf;

Func histeq(Func in, int n, int m)
{

    //Func h, hnorm, pdf;
    Var x, y, k;

    // alg: calculate hist
    // def: h_i = 1/NM * \sum_k I(k) == i; i=[0,1,2, ... 255]

    // https://halide-lang.org/docs/class_halide_1_1_r_dom.html
    RDom r(0, n, 0, m);
    h(in(r.x, r.y)) += 1.0f;
    hnorm(k) = 1.0f * h(k) / (n * m);

    // alg: pdf
    // def: A(i) += A(i-1)
    // cumsum
    RDom ri(1, 256); // ri = 1:256
    pdf(k) = hnorm(k);
    pdf(ri) += pdf(ri - 1);

    //pdf.print_loop_nest();

    // alg: equalization
    Func im_eq, im_eq8;
    im_eq(x, y) = pdf(in(x, y));
    im_eq8(x, y) = cast<uint8_t>(clamp(im_eq(x, y) * 255.0f, 0, 255));

    // schedule
    h.compute_root();
    pdf.compute_root();
    im_eq.compute_root();

    //pdf.print_loop_nest();

    // JIT-compile the pipeline for the CPU.
    //Target target = get_host_target();
    //im_eq.compile_jit( target );

    return im_eq8;
}

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        printf("Usage: ./test_eq image.png output.png \n");
        return 0;
    }

    Var x, y;
    Buffer<uint8_t> in = load_image(argv[1]);

    Func im_eq8 = histeq(Func(in), in.width(), in.height());

    Buffer<uint8_t> out(in.width(), in.height());
    im_eq8.realize(out); //<-compile jit

    //test
    Buffer<float> h = hnorm.realize({256});
    Buffer<float> H = pdf.realize({257});

    for (int t = 0; t < h.width(); t++)
    {
        //printf("t=%d\n",t);
        float asum = 0;
        for (int i = 0; i < t + 1; i++)
            asum += h(i);
        if (abs(H(t) - asum) > 0.00001)
        {
            printf("error iter %d %f != %f \n", t, H(t), asum);
            exit(-1);
        }
    }

    // save
    fprintf(stderr, "output: %s\n", argv[2]);
    save_image(out, argv[2]);

    printf("ok!!!\n");

    return 0;
}
