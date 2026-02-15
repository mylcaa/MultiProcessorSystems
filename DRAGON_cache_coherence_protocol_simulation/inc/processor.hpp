#pragma once
#include "common.hpp"

class Processor {
public:
    void issueRead(int addr) {
        instrQueue.push({InstrType::READ, addr});
    }

    void issueWrite(int addr) {
        instrQueue.push({InstrType::WRITE, addr});
    }

    bool hasInstr() const;
    Instruction nextInstr();

private:
    std::queue<Instruction> instrQueue;
};

