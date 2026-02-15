#include "processor.hpp"

bool Processor::hasInstr() const {
    bool value = !instrQueue.empty();
    return value;
}

Instruction Processor::nextInstr() {
    if (instrQueue.empty()) {
        throw std::runtime_error("Instruction queue is empty");
    }

    Instruction i = instrQueue.front();
    instrQueue.pop();
    return i;
}

