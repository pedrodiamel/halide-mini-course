#include "Halide.h"
#include <stdio.h>

using namespace Halide;
using namespace Halide::BoundaryConditions;

class Edge : public Halide::Generator<Edge>
{
public:
    Input<Buffer<uint8_t>> input{"input", 2};    // \in [W,H]
    Output<Buffer<uint8_t>> output{"output", 2}; // \in [W,H]

    void generate()
    {

        //Func definition
        Var x, y;
        Func img_pad, img16, gx, gy, g_mag, g_edge;

        /* THE ALGORITHM */
        img_pad = Halide::BoundaryConditions::repeat_edge( input );
        //img_pad = Halide::BoundaryConditions::constant_exterior(input, 0);
        //img_pad(x, y) = input(clamp(x, 0, input.width() - 1), clamp(y, 0, input.height() - 1));

        //to uin16
        img16(x, y) = cast<uint16_t>(img_pad(x, y));

        // Magnitud gradient
        gx(x, y) = (img16(x + 1, y) - img16(x - 1, y)) / 2;
        gy(x, y) = (img16(x, y + 1) - img16(x, y - 1)) / 2;
        g_mag(x, y) = gx(x, y) * gx(x, y) + gy(x, y) * gy(x, y);

        //to uin8
        g_edge(x, y) = cast<uint8_t>(g_mag(x, y));

        output(x, y) = g_edge(x, y);

        // Estimates (for autoscheduler; ignored otherwise)
        {
            input.dim(0).set_estimate(0, 4000);
            input.dim(1).set_estimate(0, 3000);
            output.set_estimate(x, 0, 4000)
                .set_estimate(y, 0, 3000);
        }

        if (!auto_schedule) {
            /* THE SCHEDULE */
            gx.compute_root();
            gy.compute_root();
            g_mag.compute_root();
            g_edge.compute_root();
        }
    }
};

HALIDE_REGISTER_GENERATOR(Edge, edge)
