
#include "Halide.h"
#include <stdio.h>
#include "mm.h"

using namespace Halide;
using namespace Halide::ConciseCasts;

class HalideMM : public Generator<HalideMM> 
{
public:
    Input<Buffer<>> ma{"MA", 2};
    Input<Buffer<>> mb{"MB", 2};
    Input<int32_t>      k{"k"};
    Output<Func> output{"output", 2};

    void generate(){
    }

    void schedule(){
        if(auto_schedule){
            // 1. Buffer : buf.dim(N).set_estimate(MIN, EXTENT)

            // 2. parameters : parm.set_estimate(VALUE)

            // 3. Func : func.set_estimate(Var, MIN, EXTENT);
        }
        else{
        }
    }
private:
    Var x, y;
};
HALIDE_REGISTER_GENERATOR(HalideMM, halide_mm);

