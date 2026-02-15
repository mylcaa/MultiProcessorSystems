#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <cassert>

constexpr int NUM_NODES = 4;
constexpr int NUM_PROCS = 4;
constexpr int CACHE_LINES = 4;

struct Cmd {
    int nodeId;
    bool ReadWrite;
    int addr;
};

struct MemoryLine {
    int address;
    int data;
};

enum class InstrType {
    READ,
    WRITE,
    NOP
};

struct Instruction {
    InstrType type;
    int address;
};

enum class BusCmd {
    NONE,
    BUS_RD,
    BUS_UPD,
    BUS_FLUSH
};

struct BusEvent {
    BusCmd cmd;
    int address;
    int sourceId;
    bool copiesExist;

    bool dataValid;
    int data;
};

enum class DragonState {
    None = 0,
    E,
    Sc,
    Sm,
    M
};

struct CacheLine {
    int tag = -1;
    int data = 0;
    DragonState state;
    bool valid;
};

enum class ProcessorCmd {
    NONE,
    PR_RD_MISS,
    PR_RD,
    PR_WR_MISS,
    PR_WR
};

struct OutstandingTrans {
    ProcessorCmd cmd;
    int address;
};