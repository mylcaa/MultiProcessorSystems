#include "snooper.hpp"

void Snooper::snoop(const BusEvent& sig,
                    Cache& cache, CacheLine& line)
{
    
    // line = cache.findLine(sig.address);
    
    // if(line.tag != -1) {
    //     sig.copiesExist = true;
    // }

    // if(sig.dataValid) {
    //     line.data = sig.data;
    // }
}
