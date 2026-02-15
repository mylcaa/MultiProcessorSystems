#pragma once
#include "common.hpp"
#include "console.hpp"

class OutstandingTransTable {
public:
    OutstandingTransTable(Console& console) : console(console) {}

    void add(ProcessorCmd cmd, int addr);
    ProcessorCmd find(int addr);
    void markComplete(int addr);
    bool hasPending(int addr) const;

private:
    Console& console;
    std::vector<OutstandingTrans> entries;
};
