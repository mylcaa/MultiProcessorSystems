#pragma once
#include "common.hpp"
#include "console.hpp"
#include "ott.hpp"
#include "write_buffer.hpp"
#include "dragon.hpp"
#include "cache.hpp"
#include "bus.hpp"

class CoherenceController {
public:
    CoherenceController(int id, Cache& cache, Console& console) : 
    fsm(console),
    nodeId(id), 
    ott(console),
    writeBuffer(console),
    cache(cache), 
    console(console)
    {}

    void processorRead(int address, Bus& bus);
    void processorWrite(int address, Bus& bus);

    void snoopBus(Bus& bus, const BusEvent& sig, bool source);
    void snoopAsResponse(Bus& bus, const BusEvent& sig);
    void snoopAsSource(Bus& bus, const BusEvent& sig);

    void step(Bus& bus, bool source);
    void stepSource(Bus& bus);
    void stepResponse(Bus& bus);

    bool WriteBackBufferEmpty(bool instruction);

    DragonFSM fsm;

private:
    int nodeId;

    OutstandingTransTable ott;
    WriteBuffer writeBuffer;

    Cache& cache;
    Console& console;
};
