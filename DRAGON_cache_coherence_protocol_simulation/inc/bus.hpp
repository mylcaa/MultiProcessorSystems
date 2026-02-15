#pragma once
#include "common.hpp"
#include "console.hpp"

class Bus {
public:
    Bus(Console& console);
    void request(const BusEvent& sig);
    bool busy() const;

    const BusEvent& current() const;

    void step();
    void clear();

    void setCopiesExistLine(bool exist);
    void arbitrate(int numNodes, std::vector<int>& arbOrder);

private:
    Console& console;

    BusEvent signals;
    bool occupied;
};
