#include "Halide.h"
#include <stdio.h>

using namespace Halide;

// lscpu

Expr fun(Var x, Var y)
{
    return x + y + select(x < y, 1, 0);
}

Func alg(float offset)
{
    // algorithm
    Func g("g");
    Var x("x"), y("y");
    g(x, y) = fun(x, y) + offset;

    // scheduler
    Var x0, y0, xi, yi;
    g.compute_root()
        .tile(x, y, x0, y0, xi, yi, 4, 4)
        .parallel(y0)
        .vectorize(xi);

    return g;
}

int main(int argc, char const *argv[])
{

    if(argc < 2){
        printf("Usage: ./test offset \n");
        return 0;
    }

    // def var, func, exp
    //Var x("x"),y("y"),c("c");
    //Func f("f"), g("g"), d("d");
    Func f("f");
    float offset = atof(argv[1]);

    f.trace_stores();

    // alg
    // f(x,y) = fun(x,y) + offset;
    f = alg(offset);

    //schedule
    // f.compute_root()

    //compiler
    Buffer<float> out;
    out = f.realize({512, 512}); // <- compiler

    f.print_loop_nest();

    // HL_DEBUG_CODEGEN=2
#ifdef HL_DEBUG_PROCESS
    f.compile_to_lowered_stmt("bin/f.html", {}, HTML);
#endif

    // test
    {
        //for y:
        // for x

        for (int y = 0; y < out.height(); y++)
        {
            for (int x = 0; x < out.width(); x++)
            {
                /* code */
                if (out(x, y) != (x + y + (x < y) + offset))
                {
                    printf("error %f != %f", out(x, y), (x + y + (x < y) + offset));
                    exit(-1);
                }
            }

            /* code */
        }
    }

    printf("ok!!\n");
    return 0;
}
