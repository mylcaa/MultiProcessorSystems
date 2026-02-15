#pragma once
#include "common.hpp"
#include "console.hpp"

class DragonFSM {
public:
    DragonFSM(Console& console): console(console) {}

    BusCmd generateBusCmd(DragonState& cur_state, ProcessorCmd&& processor_event);
    BusCmd generateBusCmd(DragonState&& cur_state, ProcessorCmd&& processor_event);
    BusCmd handleBusCmd(CacheLine& line, const BusCmd& bus_event);
    BusCmd handleBusCmd(CacheLine& line, bool C, ProcessorCmd processorCmd);

    std::string print(BusCmd state);
private:
    void PrRdMiss(bool C, CacheLine& line);
    void PrWrMiss(bool C, CacheLine& line);
    void PrWr(bool C, CacheLine& line);
    void BusRd(CacheLine& line);
    void BusUpd(CacheLine& line);


    std::string print(DragonState state);
    std::string print(ProcessorCmd cmd);

    Console& console;
};