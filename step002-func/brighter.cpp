
#include "Halide.h"
#include "halide_image_io.h"

#include <stdio.h>

using namespace Halide;
using namespace Halide::Tools;

int main(int argc, char const *argv[])
{

    if(argc < 4){
        printf("Usage: ./test image.png brighter output.png \n");
        return 0;
    }

    fprintf(stderr, "input: %s\n", argv[1]);
    Buffer<uint8_t> in = load_image(argv[1]);

    // definition
    Var x, y, c;
    Func f, g, h, fout8;
    float d = (float)atof(argv[2]);

    // alg
    //// brighter

    // boundary conditions

    // Expr clamped_x = clamp(x, 0, in.width()-1);
    // Expr clamped_y = clamp(y, 0, in.height()-1);
    // clamped(x, y, c) = in(clamped_x, clamped_y, c);
    // Halide::BoundaryConditions::repeat_edge( input );
    // Halide::BoundaryConditions::constant_exterior(input, 0);

    f(x, y, c) = min(in(x, y, c) * d, 255);

    // g(x,y,c) = select( c==0, f(x,y,c)*1,  //R
    //                    c==1, f(x,y,c)*0,  //G
    //                          f(x,y,c)*0   //B
    //                 );

    //Func R,G,B;
    //R(x,y) = f(x,y,0);

    RDom r1(10, 100, 10, 100);
    g(x, y, c) = f(x, y, c);
    g(r1.x, r1.y, c) = (g(r1.x, r1.y, c) * 2.0f + 10.0f) / 2.0f;

    RDom r2(0, 50, 0, 50);
    r2.where(r2.x + r2.y >= 25);
    h(x, y, c) = g(x, y, c);
    h(r2.x + 200, r2.y + 200, c) = 0.0f;

    fout8(x, y, c) = cast<uint8_t>(h(x, y, c));

    // schedule
    f.compute_root();
    g.compute_root();
    h.compute_root();

    // compile
    Buffer<uint8_t> out(in.width(), in.height(), in.channels());
    fout8.realize({out});

    // test
    // code here ...

    // save
    fprintf(stderr, "output: %s\n", argv[3]);
    save_image(out, argv[3]);

    printf("Ok!!! ... \n");

    return 0;
}
