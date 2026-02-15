#include "write_buffer.hpp"

void WriteBuffer::push(BusEvent event, bool instruction) {
    if(instruction) {
        std::cout << "Bus busy instruction pushed to WB buffer for INSTRUCTIONS" << std::endl;
        console.log("Bus busy instruction pushed to WB buffer for INSTRUCTIONS", RED);
        initiateBuf.push_back(event);
    } else {
        std::cout << "Bus busy data for flush pushed to WB buffer for DATA WRITEBACKS" << std::endl;
        console.log("Bus busy data for flush pushed to WB buffer for DATA WRITEBACKS", RED);
        respondBuf.push_back(event);
    }
}

bool WriteBuffer::empty(bool instruction) const {
    if(instruction) {
        return initiateBuf.empty();
    } else {
        return respondBuf.empty();
    }
}

BusEvent WriteBuffer::pop(bool instruction) {
    BusEvent e;

    if(instruction) {
        e = initiateBuf.front();
        initiateBuf.erase(initiateBuf.begin());
    } else {
        e = respondBuf.front();
        respondBuf.erase(respondBuf.begin());
    }

    return e;
}

