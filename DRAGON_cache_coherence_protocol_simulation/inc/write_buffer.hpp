#pragma once
#include "common.hpp"
#include "console.hpp"

class WriteBuffer {
public:
    WriteBuffer(Console& console) : console(console) {}

    void push(BusEvent event, bool instruction);
    bool empty(bool instruction) const;
    BusEvent pop(bool instruction);

private:
    Console& console;
    std::vector<BusEvent> initiateBuf;
    std::vector<BusEvent> respondBuf;
};
