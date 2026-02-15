#include "dragon.hpp"

void DragonFSM::PrRdMiss(bool C, CacheLine& line) {
    std::cout << "1. PrRdMiss event" << std::endl;
    std::cout << "2. COPIES_EXIST bus line = " << C << std::endl;
    std::cout << "3. Current ";

    console.log("1. PrRdMiss event", YELLOW);
    console.log("2. COPIES_EXIST bus line = " + std::to_string(C), YELLOW);
    console.log("3. Current " + print(line.state), YELLOW);
    
    line.state = C? DragonState::Sc : DragonState::E;
    
    std::cout << "      => New ";
    console.log("      => New " + print(line.state), YELLOW);
}

void DragonFSM::PrWrMiss(bool C, CacheLine& line) {
    std::cout << "1. PrWrMiss event" << std::endl;
    std::cout << "2. COPIES_EXIST bus line =" << C << std::endl;
    std::cout << "3. Current ";
    
    console.log("1. PrWrMiss event", YELLOW);
    console.log("2. COPIES_EXIST bus line = " + std::to_string(C), YELLOW);
    console.log("3. Current " + print(line.state), YELLOW);

    line.state = C? DragonState::Sm : DragonState::M;

    std::cout << "      => New ";
    console.log("      => New " + print(line.state), YELLOW);
}

void DragonFSM::PrWr(bool C, CacheLine& line) {
    std::cout << "1. PrWr event" << std::endl;
    std::cout << "2. COPIES_EXIST bus line =" << C << std::endl;
    std::cout << "3. Current ";
    
    console.log("1. PrWr event", YELLOW);
    console.log("2. COPIES_EXIST bus line = " + std::to_string(C), YELLOW);
    console.log("3. Current " + print(line.state), YELLOW);
     
    switch (line.state) {
        case DragonState::Sc:
            line.state = C? DragonState::Sm : DragonState::M;
            break;
        case DragonState::Sm:
            if (!C) {
                line.state = DragonState::M;
            }
            break;
        default:
            break;
    }

    std::cout << "      => New ";
    console.log("      => New " + print(line.state), YELLOW);
}

void DragonFSM::BusRd(CacheLine& line) {
    std::cout << "1. BusRd event" << std::endl;
    std::cout << "2. Current ";
    
    console.log("1. BusRd event", YELLOW);
    console.log("2. Current " + print(line.state), YELLOW);

    if (line.state == DragonState::M) {
        line.state = DragonState::Sm;
    }
    else if (line.state == DragonState::E) {
        line.state = DragonState::Sc;
    }

    std::cout << "      => New ";
    console.log("      => New " + print(line.state), YELLOW);
}

void DragonFSM::BusUpd(CacheLine& line) {
    std::cout << "1. BusUpd event" << std::endl;
    std::cout << "2. Current ";
    
    console.log("1. BusUpd event", YELLOW);
    console.log("2. Current " + print(line.state), YELLOW);

    if (line.state == DragonState::Sm) {
        line.state = DragonState::Sc;
    }

    std::cout << "      => New ";
    console.log("      => New " + print(line.state), YELLOW);
}

BusCmd DragonFSM::generateBusCmd(DragonState& cur_state, ProcessorCmd&& processor_event) {
    std::cout << "1. Current ";
    console.log("1. Current " + print(cur_state), YELLOW);
    std::cout << "2. Issued ";
    console.log("2. Issued " + print(processor_event), YELLOW);

    BusCmd event = BusCmd::NONE;

    switch (processor_event) {
        case ProcessorCmd::PR_WR_MISS:
            event = BusCmd::BUS_RD;
        break;
        
        case ProcessorCmd::PR_WR:
            if (cur_state == DragonState::Sc || cur_state == DragonState::Sm) {
                event = BusCmd::BUS_UPD;
            }

            if (cur_state == DragonState::E) {
                cur_state = DragonState::M;

                std::cout << "      => New ";
                console.log("      => New " + print(cur_state), YELLOW);
            }
        break;

        case ProcessorCmd::PR_RD_MISS:
            event = BusCmd::BUS_RD;
        break;

        case ProcessorCmd::PR_RD:
        break;

        default:
        break;
    }

    std::cout << "      => Generated ";
    console.log("      => Generated " + print(event), YELLOW);

    return event;
}

BusCmd DragonFSM::generateBusCmd(DragonState&& cur_state, ProcessorCmd&& processor_event) {
    std::cout << "1. Current ";
    console.log("1. Current " + print(cur_state), YELLOW);
    std::cout << "2. Issued ";
    console.log("2. Issued " + print(processor_event), YELLOW);

    BusCmd event = BusCmd::NONE;

    switch (processor_event) {
        case ProcessorCmd::PR_WR_MISS:
            event = BusCmd::BUS_RD;
        break;
        
        case ProcessorCmd::PR_WR:
            if (cur_state == DragonState::Sc || cur_state == DragonState::Sm) {
                event = BusCmd::BUS_UPD;
            }

            if (cur_state == DragonState::E) {
                cur_state = DragonState::M;

                std::cout << "      => New ";
                console.log("      => New " + print(cur_state), YELLOW);
            }
        break;

        case ProcessorCmd::PR_RD_MISS:
            event = BusCmd::BUS_RD;
        break;

        case ProcessorCmd::PR_RD:
        break;

        default:
        break;
    }

    std::cout << "      => Generated ";
    console.log("      => Generated " + print(event), YELLOW);

    return event;
}

BusCmd DragonFSM::handleBusCmd(CacheLine& line, const BusCmd& bus_event) {
    BusCmd flush_event = BusCmd::NONE;

    switch (bus_event) {
        case BusCmd::BUS_RD:
            if (line.state == DragonState::M || line.state == DragonState::Sm) {
                flush_event = BusCmd::BUS_FLUSH;
            }

            BusRd(line);

            std::cout << "      => Generated ";
            console.log("      => Generated " + print(flush_event), YELLOW);
        break;
        
        case BusCmd::BUS_UPD:
            BusUpd(line);
        break;

        default:
        break;
    }

    return flush_event;
}

BusCmd DragonFSM::handleBusCmd(CacheLine& line, bool C, ProcessorCmd processorCmd) {
    BusCmd event = BusCmd::NONE;

    switch (processorCmd) {
        case ProcessorCmd::PR_RD_MISS:
            PrRdMiss(C, line);
        break;

        case ProcessorCmd::PR_WR_MISS:
            PrWrMiss(C, line);

            if(C) {
                event = BusCmd::BUS_UPD;
                line.valid = false; //it will be valid once the processor writes to the line in cache

                std::cout << "      => Generated ";
                console.log("      => Generated " + print(event), YELLOW);
            }
        break;

        case ProcessorCmd::PR_WR:
            PrWr(C, line);
        break;

        default:
        break;
    }
    return event;
}

std::string DragonFSM::print(DragonState state) {
    std::cout << "FSM state = ";
    std::string state_string = "FSM state = ";

    switch(state) {
        case DragonState::None:
        std::cout << "NONE" << std::endl;
        state_string += "NONE";
        break;

        case DragonState::E:
        std::cout << "E" << std::endl;
        state_string += "E";
        break;

        case DragonState::M:
        std::cout << "M" << std::endl;
        state_string += "M";
        break;

        case DragonState::Sc:
        std::cout << "Sc" << std::endl;
        state_string += "Sc";
        break;

        case DragonState::Sm:
        std::cout << "Sm" << std::endl;
        state_string += "Sm";
        break;
    }

    return state_string;
}

std::string DragonFSM::print(BusCmd state) {
    std::cout << "bus command = ";
    std::string state_string = "bus command = ";

    switch(state) {
        case BusCmd::NONE:
        std::cout << "NONE" << std::endl;
        state_string += "NONE";
        break;

        case BusCmd::BUS_FLUSH:
        std::cout << "BUS_FLUSH" << std::endl;
        state_string += "BUS_FLUSH";
        break;

        case BusCmd::BUS_RD:
        std::cout << "BUS_RD" << std::endl;
        state_string += "BUS_RD";
        break;

        case BusCmd::BUS_UPD:
        std::cout << "BUS_UPD" << std::endl;
        state_string += "BUS_UPD";
        break;
    }

    return state_string;
}

std::string DragonFSM::print(ProcessorCmd cmd) {
    std::cout << "processor command = ";
    std::string cmd_string = "processor command = ";

    switch(cmd) {
        case ProcessorCmd::PR_WR:
        std::cout << "PrWr" << std::endl;
        cmd_string += "PrWr";
        break;

        case ProcessorCmd::PR_RD:
        std::cout << "PrRd" << std::endl;
        cmd_string += "PrRd";
        break;

        case ProcessorCmd::PR_WR_MISS:
        std::cout << "PrWrMiss" << std::endl;
        cmd_string += "PrWrMiss";
        break;

        case ProcessorCmd::PR_RD_MISS:
        std::cout << "PrRdMiss" << std::endl;
        cmd_string += "PrRdMiss";
        break;

        case ProcessorCmd::NONE:
        std::cout << "/" << std::endl;
        break;
    }

    return cmd_string;
}