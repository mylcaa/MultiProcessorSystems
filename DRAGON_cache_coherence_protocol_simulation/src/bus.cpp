#include "bus.hpp"

Bus::Bus(Console& console): console(console), signals({BusCmd::NONE, -1, -1, false, false, 0}),
                occupied(false) {}

void Bus::request(const BusEvent& sig) {
    std::cout << "Instruction pushed to bus" << std::endl;
    console.log("Instruction pushed to bus", RED);
    signals = sig;
    occupied = true;
}

bool Bus::busy() const {
    return occupied;
}

const BusEvent& Bus::current() const {
    return signals;
}

void Bus::step() {
    // One-cycle broadcast model
    // Bus stays valid for one cycle
}

void Bus::clear() {
    std::cout << "bus clear" << std::endl;  
    console.log("Bus clear", RED);
    occupied = false;
    signals.cmd = BusCmd::NONE;
}

void Bus::setCopiesExistLine(bool exist) {
    std::cout << "Set copies exist line: " << exist << std::endl;
    console.log("Set copies exist line: " + std::to_string(exist), RED);
    signals.copiesExist = exist;
}

void Bus::arbitrate(int numNodes, std::vector<int>& arbOrder) {
    if (signals.cmd == BusCmd::NONE) {
        return;
    }

    int temp = arbOrder[0];
    for(int i = 0; i < numNodes-1; ++i) {
        arbOrder[i] = arbOrder[i+1];
    }
    arbOrder[numNodes-1] = temp;
}

