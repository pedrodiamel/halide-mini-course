#include "Halide.h"
#include "halide_image_io.h"

#include <stdio.h>

using namespace Halide;
using namespace Halide::Tools;

Func hist(Func in, int n, int m)
{
    Var t;
    Func h, hn;
    RDom r(0, n, 0, m);
    h(in(r.x, r.y)) += 1.0f;
    hn(t) = (1.0f * h(t)) / (n * m);

    h.compute_root();
    return hn;
}

Func cumsum(Func p, int n)
{
    Func s;
    Var k;
    RDom ri(1, n);
    s(k) = p(k);
    s(ri) += s(ri - 1);

    s.compute_root();
    return s;
}

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        printf("Usage: ./test_th image.png output.png \n");
        return 0;
    }

    Buffer<uint8_t> in = load_image(argv[1]);

    Var x, y, t;
    Func p, p1, p2, mu, muS, muG, f_th;
    RDom ri(0, 256);

    Func im;
    im(x, y) = in(x, y, 0);

    // probability
    p = hist(im, in.width(), in.height());
    p1 = cumsum(p, 255);

    mu(t) = 0.0f;
    mu(ri) = ri * p(ri);
    muS = cumsum(mu, 255);

    muG(t) = 0.0f;
    muG(ri) = sum(mu(ri)) * p1(ri);

    p1.compute_root();
    mu.compute_root();
    muG.compute_root();

    Tuple th = argmax(((muG(ri) - muS(ri)) * (muG(ri) - muS(ri))) / (p1(ri) * (1.0f - p1(ri)) + 0.00001f));

    //thresholding
    f_th(x, y) = cast<uint8_t>((im(x, y) > th[0])) * 255;

    Buffer<uint8_t> out(in.width(), in.height());
    f_th.realize(out);

    // save
    fprintf(stderr, "output: %s\n", argv[2]);
    save_image(out, argv[2]);

    printf("ok!!! ... \n");

    return 0;
}
