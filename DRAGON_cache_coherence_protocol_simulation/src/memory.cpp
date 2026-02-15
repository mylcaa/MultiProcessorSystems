#include "memory.hpp"

#include <random>
#include <algorithm>

void Memory::fillMemory(int address) {
    static std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0, 1023);

    //if address already exists in memory ignore it
    for(auto line : data) {
        if(line.address == address) {
            return;
        }
    }

    MemoryLine line;
    line.address = address;
    line.data = dist(rng);

    data.push_back(line);
    std::cout << "memory[" << address << "] = " << line.data << std::endl;
    console.log("memory[" + std::to_string(address) + "] = " + std::to_string(line.data), BLUE);
           
}

void Memory::snoopBus(Bus& bus, const BusEvent& sig) {
    std::cout << "Main memory snoop" << std::endl;
    console.log("Main memory snoop", BLUE);
    if (sig.cmd != BusCmd::BUS_RD) {
        std::cout << "Nothing to be snooped" << std::endl;
        console.log("Nothing to be snooped", BLUE);
        return;
    }

    std::cout << "For BUS_RD find data and send to bus" << std::endl;
    console.log("For BUS_RD find data and send to bus", BLUE);
    auto it = std::find_if(
        data.begin(),
        data.end(),
        [&](const MemoryLine& line) {
            return line.address == sig.address;
        }
    );

    if (it != data.end()) {
        BusEvent response = sig;

        response.cmd       = BusCmd::NONE;
        response.dataValid = true;
        response.data      = it->data;

        bus.request(response);
    }
}

void Memory::step(Bus& bus) {
    const BusEvent& current = bus.current();
    snoopBus(bus, current);
}
