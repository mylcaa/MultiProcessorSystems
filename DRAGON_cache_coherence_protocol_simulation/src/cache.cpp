#include "cache.hpp"

Cache::Cache(Console& console) : console(console) {
    for (int i = 0; i < CACHE_LINES; ++i) {
        lines[i] = {-1, 0, DragonState::None, false};
    }
}

CacheLine Cache::findLine(int address) {
    for (int i = 0; i < CACHE_LINES; i++) {
         if (lines[i].tag == address && lines[i].state != DragonState::None) {
            std::cout << "CACHE: HIT tag " << address << " state = " << (int)lines[i].state << std::endl;
            console.log("CACHE: HIT tag " + std::to_string(address), BLUE);
            return lines[i];
        }
    }

    std::cout << "CACHE: MISS tag " << address << std::endl;
    console.log("CACHE: MISS tag " + std::to_string(address), BLUE);
    return {-1, 0, DragonState::None, false};
}

CacheLine Cache::lookUpLine(int address) {
    for (int i = 0; i < CACHE_LINES; i++) {
        if (lines[i].tag == address) {
            return lines[i];
        }
    }
    return {-1, 0, DragonState::None, false};
}

int Cache::allocateLine(CacheLine line) {
    for (int i = 0; i < CACHE_LINES; i++) {
        if (!lines[i].valid) {
            lines[i].valid = line.valid;
            lines[i].tag = line.tag;
            lines[i].state = line.state;
            lines[i].data = line.data;

            std::cout << "Allocate line " << i << " for tag " << line.tag << std::endl;
            console.log("Allocate line " + std::to_string(i) + " for tag " + std::to_string(line.tag), BLUE);
            return i;
        }
    }

    // Simple replacement: evict line 0
    std::cout << "Allocate line by evicting line 0 for tag " << line.tag << std::endl;
    console.log("Allocate line by evicting line 0 for tag  " + std::to_string(line.tag), BLUE);
            
    lines[0].tag = line.tag;
    lines[0].data = line.data;
    lines[0].state = line.state;
    lines[0].valid = line.valid;
    return 0;
}

void Cache::placeLine(CacheLine line) {
    for (int i = 0; i < CACHE_LINES; i++) {
        if (lines[i].tag == line.tag) {
            std::cout << "Place updated line with tag " << line.tag << " in cache line = " << i << std::endl;        
            console.log("Place updated line with tag " + std::to_string(line.tag) + " in cache line = " + std::to_string(i), BLUE);
            lines[i] = line;
            return;
        }
    }

    std::cout << "ERROR: No tag " << line.tag << " exists in cache currently" << std::endl;        
    console.log("ERROR: No tag " + std::to_string(line.tag) + " exists in cache currently", BLUE);
            
}
