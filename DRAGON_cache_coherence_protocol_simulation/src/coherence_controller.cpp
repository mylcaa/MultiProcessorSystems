#include "coherence_controller.hpp"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

std::uniform_int_distribution<std::mt19937::result_type> dist(0, 100);

void CoherenceController::processorRead(int addr, Bus& bus) {
    std::cout << "Proc" << nodeId << ": PrRd" << std::endl;
    console.log("Proc" + std::to_string(nodeId) + ": PrRd\n", GREEN);

    CacheLine line = cache.findLine(addr);

    if (line.tag != -1) {
        return;
    }

    //allocate space in cache
    cache.allocateLine({addr, 0, DragonState::None, false});

    BusCmd busEvent = fsm.generateBusCmd(DragonState::None, ProcessorCmd::PR_RD_MISS);
    ott.add(ProcessorCmd::PR_RD_MISS, addr);
    
    if(bus.busy()) {
        writeBuffer.push({busEvent, addr, nodeId, false, false, 0}, true);
    } else {  
        bus.request({busEvent, addr, nodeId, false, false, 0});
    }
}

void CoherenceController::processorWrite(int addr, Bus& bus) {
    std::cout << "Proc" << nodeId << " : PrWr" << std::endl;
    console.log("Proc" + std::to_string(nodeId) + ": PrWr\n", GREEN);

    BusCmd busEvent;
    ProcessorCmd processorEvent;
    
    CacheLine line = cache.findLine(addr);

    if (line.tag != -1) {
        //in this case we had a PR_WR and the processor wrote to the cache - data needs to be changed    
        line.valid = true;
        line.data = dist(rng);
        cache.placeLine(line);

        std::cout << "Proc " << nodeId << " writes to cache line " << line.tag << " => " << line.data << std::endl;
        console.log("Proc " + std::to_string(nodeId) + " writes to cache line " + std::to_string(line.tag) + " => " + std::to_string(line.data), GREEN);

        processorEvent = ProcessorCmd::PR_WR;
        busEvent = fsm.generateBusCmd(line.state, ProcessorCmd::PR_WR);

        cache.placeLine(line);
    } else {
        //allocate space in cache
        cache.allocateLine({addr, 0, DragonState::None, false});

        processorEvent = ProcessorCmd::PR_WR_MISS;
        busEvent = fsm.generateBusCmd(DragonState::None, ProcessorCmd::PR_WR_MISS);
    }

    if(busEvent == BusCmd::NONE) {
        return;
    }

    ott.add(processorEvent, addr);

    bool dataValid = false;
    int data = 0;

    if(busEvent == BusCmd::BUS_UPD) {
        dataValid = true;
        data = line.data;
    }

    if (bus.busy()) {
        writeBuffer.push({busEvent, addr, nodeId, false, dataValid, data}, true);
    } else {
        bus.request({busEvent, addr, nodeId, false, dataValid, data});
    }
}

void CoherenceController::snoopBus(Bus& bus, const BusEvent& sig, bool source) {
    std::cout << "Proc" << nodeId << " snoop" << std::endl;
    console.log("Proc" + std::to_string(nodeId) + " snoop\n", GREEN);

    if (source) {
        snoopAsSource(bus, sig);
    } else {
        if(sig.cmd == BusCmd::NONE) {
            std::cout << "Nothing to be snooped" << std::endl;
            console.log("Nothing to be snooped\n", GREEN);
            return;
        }
        snoopAsResponse(bus, sig);
    }
}

void CoherenceController::snoopAsResponse(Bus& bus, const BusEvent& sig) {
    CacheLine line;
    line = cache.findLine(sig.address);
    
    if(line.tag != -1) {
        bus.setCopiesExistLine(true);
    } else {
        return;
    }

    if(sig.dataValid) {
        line.data = sig.data;
    }

    BusCmd busEvent = fsm.handleBusCmd(line, sig.cmd);
    cache.placeLine(line);

    if(busEvent == BusCmd::BUS_FLUSH) {
        writeBuffer.push({busEvent, sig.address, nodeId, sig.copiesExist, true, line.data}, false);
    }
}

void CoherenceController::snoopAsSource(Bus& bus, const BusEvent& sig) {
    ProcessorCmd ottInput = ott.find(sig.address);
    if(ottInput == ProcessorCmd::NONE) {
        return;
    }

    CacheLine line;
    line = cache.lookUpLine(sig.address);

    if(sig.cmd == BusCmd::BUS_FLUSH && sig.dataValid) {
        line.data = sig.data;
    }

    BusCmd busEvent = fsm.handleBusCmd(line, sig.copiesExist, ottInput);
    bus.setCopiesExistLine(false);


    cache.placeLine(line);
    ott.markComplete(sig.address);

    //Processor writes new value if it was a PrWrMiss
    //this is an imitation of that process
    if(busEvent == BusCmd::BUS_UPD) {
        line.valid = true;
        line.data = dist(rng);

        cache.placeLine(line);

        std::cout << "Proc " << nodeId << " writes to cache line " << line.tag << std::endl;
        console.log("Proc " + std::to_string(nodeId) + " writes to cache line " + std::to_string(line.tag) + " => " + std::to_string(line.data), GREEN);
    }

    line = cache.lookUpLine(sig.address);
    if(busEvent == BusCmd::BUS_UPD && line.valid == true) {
        writeBuffer.push({busEvent, sig.address, nodeId, false, true, line.data}, true);
    }
}

void CoherenceController::step(Bus& bus, bool source) {
    if(source) {
        stepSource(bus);
    } else {
        stepResponse(bus);
    }
}

void CoherenceController::stepSource(Bus& bus) {
    if (!writeBuffer.empty(true) && !bus.busy()) {
        auto e = writeBuffer.pop(true);

        std::cout << "Coherence controller " << nodeId << " issues ";
        console.log("Coherence controller " + std::to_string(nodeId) + " issues ", GREEN);
        console.log(fsm.print(e.cmd) + " for tag " + std::to_string(e.address), GREEN);
        std::cout << "for tag " << e.address << std::endl;

        bus.request(e);
    }
}

void CoherenceController::stepResponse(Bus& bus) {
    // respond with data if FLUSH evoked in snoop
    if (!writeBuffer.empty(false)) {
        std::cout << "Coherence controller " << nodeId << " issues bus command = BUS_FLUSH" << std::endl;
        console.log("Coherence controller " + std::to_string(nodeId) + " issues bus command = BUS_FLUSH\n", GREEN);
        auto e = writeBuffer.pop(false);
        bus.request(e);
    }
}

bool CoherenceController::WriteBackBufferEmpty(bool instruction) {
    return writeBuffer.empty(instruction);
}
