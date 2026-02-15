#pragma once
#include "common.hpp"
#include "console.hpp"

class Cache {
public:
    Cache(Console& console);

    CacheLine& line(int idx) { return lines[idx]; }

    CacheLine findLine(int address);
    CacheLine lookUpLine(int address);
    int allocateLine(CacheLine line);
    void placeLine(CacheLine line);

private:
    Console& console;
    CacheLine lines[CACHE_LINES];
};

