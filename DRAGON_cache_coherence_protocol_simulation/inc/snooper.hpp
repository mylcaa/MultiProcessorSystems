#pragma once
#include "common.hpp"
#include "cache.hpp"

class Snooper {
public:
    void snoop(const BusEvent& sig, Cache& cache, CacheLine& line);
};
