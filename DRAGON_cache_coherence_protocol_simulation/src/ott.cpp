#include "ott.hpp"

void OutstandingTransTable::add(ProcessorCmd cmd, int addr) {
    std::cout << "Instruction added to outstanding transaction table" << std::endl;
    console.log("Instruction added to outstanding transaction table", ORANGE);
    entries.push_back({cmd, addr});
}

ProcessorCmd OutstandingTransTable::find(int addr) {
    std::cout << "Find tag " << addr << " in outstanding transaction table..." << std::endl;
    console.log("Find tag " + std::to_string(addr) + " in outstanding transaction table...", ORANGE);
    for (auto& e : entries) {
        if (e.address == addr) {
            std::cout << "Found!" << std::endl;
            console.log("Found!", ORANGE);
            return e.cmd;
        }
    }
    std::cout << "Does not exist here" << std::endl;
    console.log("Does not exist here", ORANGE);
    return ProcessorCmd::NONE;
}

void OutstandingTransTable::markComplete(int addr) {
    std::cout << "Mark tag " << addr << " in table as complete" << std::endl;
    console.log("Mark tag " + std::to_string(addr) + " in table as complete", ORANGE);
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->address == addr) {
            entries.erase(it);
            return;
        }
    }
}

bool OutstandingTransTable::hasPending(int addr) const {
    for (const auto& e : entries) {
        if (e.address == addr)
            return true;
    }
    return false;
}
