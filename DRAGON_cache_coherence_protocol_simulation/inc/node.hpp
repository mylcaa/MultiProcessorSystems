#pragma once
#include "common.hpp"
#include "processor.hpp"
#include "cache.hpp"
#include "coherence_controller.hpp"
#include "console.hpp"

class Node {
public:
    Node(int id, Console& console);

    Processor cpu;
    Cache cache;
    CoherenceController cc;

    int id;
};