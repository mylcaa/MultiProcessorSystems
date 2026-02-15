#pragma once
#include "common.hpp"
#include "bus.hpp"
#include "console.hpp"

class Memory {
public:
    Memory(Console& console) : console(console) {}

    void fillMemory(int address);
    void snoopBus(Bus& bus, const BusEvent& sig);
    void step(Bus& bus);

private:
    std::vector<MemoryLine> data;
    Console& console;
};