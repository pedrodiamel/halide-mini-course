#include "Halide.h"
#include "halide_image_io.h"

#include <stdio.h>

using namespace Halide;
using namespace Halide::Tools;

int main(int argc, char const *argv[])
{

    if (argc < 3)
    {
        printf("Usage: ./test_blur image.png output.png \n");
        return 0;
    }

    Buffer<uint8_t> in = load_image(argv[1]);

    Var x, y, c;
    Func clamped("clamped");

    // boundary condition

    // Expr clamped_x = clamp(x, 0, in.width()-1);
    // Expr clamped_y = clamp(y, 0, in.height()-1);
    // clamped(x, y, c) = in(clamped_x, clamped_y, c);
    clamped = Halide::BoundaryConditions::repeat_edge(in);
    //clamped = Halide::BoundaryConditions::constant_exterior(in, 0);

    Func input_16("input");
    input_16(x, y, c) = cast<uint16_t>(clamped(x, y, c));

    // Blur it horizontally:
    Func blur_x("blur_x");
    blur_x(x, y, c) = (input_16(x - 1, y, c) +
                       2 * input_16(x, y, c) +
                       input_16(x + 1, y, c)) /
                       4;

    // Blur it vertically:
    Func blur_y("blur_y");
    blur_y(x, y, c) = (blur_x(x, y - 1, c) +
                       2 * blur_x(x, y, c) +
                       blur_x(x, y + 1, c)) /
                       4;

    // Convert back to 8-bit.
    Func out8("output");
    out8(x, y, c) = cast<uint8_t>(blur_y(x, y, c));

    blur_x.compute_root();

    Buffer<uint8_t> out(in.width(), in.height(), in.channels());
    out8.realize(out);

    // save
    fprintf(stderr, "output: %s\n", argv[2]);
    save_image(out, argv[2]);

    printf("ok!!! ... \n");

    return 0;
}
