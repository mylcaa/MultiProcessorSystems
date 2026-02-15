#include "raylib.h"

#include "common.hpp"
#include "processor.hpp"
#include "cache.hpp"
#include "coherence_controller.hpp"
#include "bus.hpp"
#include "node.hpp"
#include "memory.hpp"

#include "visual.hpp"
#include "console.hpp"
#include "draw_dragon_fsm.hpp"

int main() {

    // Raylib init
    InitWindow(1800, 1800, "Dragon Cache Coherence Simulator");
    SetTargetFPS(60);

    // System components
    VisualLayout layout;
    Console console;

    Node nodes[NUM_NODES] = { Node(0, console), Node(1, console), Node(2, console), Node(3, console)};
    std::vector<int> arbOrder = {0, 1, 2, 3};
    Bus bus(console);
    Memory memory(console);

    std::vector<Cmd> commands;

    bool advanceCycle = false;
    bool instructionsAdded = false;
    int cycle = 0;

    // Main loop
    while (!WindowShouldClose()) {

        // Input: single-step execution
        if (IsKeyPressed(KEY_SPACE) && instructionsAdded) {
            advanceCycle = true;
        }

        // Simulation step (1 cycle)
        if (advanceCycle) {
            advanceCycle = false;
            cycle++;

            std::cout << "\nCYCLE " << cycle << std::endl << std::endl;
            console.log("-------------------------------------------------------------------------------", WHITE);
            console.log("CYCLE  " + std::to_string(cycle), WHITE);
            console.log("-------------------------------------------------------------------------------", WHITE);

            for(int i = 0; i < NUM_NODES; ++i) {
                std::cout << "arbitrationOrder[" << i << "] = " << arbOrder[i] << std::endl;
                console.log("arbitrationOrder[" + std::to_string(i) + "] = " + std::to_string(arbOrder[i]), WHITE);
            }

            // 1) Processors issue at most one instruction
            std::cout << "\nStep 1) Processors issue one instruction" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 1) Processors issue one instruction", WHITE);
            console.log("----------------------------------------", WHITE);

            for (int i = 0; i < NUM_NODES; i++) {
                if (nodes[arbOrder[i]].cpu.hasInstr() && nodes[arbOrder[i]].cc.WriteBackBufferEmpty(true)) {
                    Instruction instr = nodes[arbOrder[i]].cpu.nextInstr();
                    if (instr.type == InstrType::READ) {
                        nodes[arbOrder[i]].cc.processorRead(
                            instr.address,
                            bus
                        );
                    } else {
                        nodes[arbOrder[i]].cc.processorWrite(
                            instr.address,
                            bus
                        );
                    }
                } else {
                    //issue instruction from wb_instruction
                    nodes[arbOrder[i]].cc.step(bus, true);
                }
            }

            // 2) Broadcast bus transaction
            std::cout << "\nStep 2) Broadcast bus transaction" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 2) Broadcast bus transaction", WHITE);
            console.log("----------------------------------------", WHITE);
            bus.step();
            bus.arbitrate(NUM_NODES, arbOrder);

            for(int i = 0; i < NUM_NODES; ++i) {
                std::cout << "arbitrationOrder[" << i << "] = " << arbOrder[i] << std::endl;
                console.log("arbitrationOrder[" + std::to_string(i) + "] = " + std::to_string(arbOrder[i]), WHITE);
            }

            // 3) Snooping phase
            std::cout << "\nStep 3) Snooping phase" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 3) Snooping phase", WHITE);
            console.log("----------------------------------------", WHITE);

            for (int i = 0; i < NUM_NODES-1; i++) {
                nodes[arbOrder[i]].cc.snoopBus(
                    bus, bus.current(), false
                );
            }
            //Main memory responds if it needs to
            memory.step(bus);

            // 4) Controllers handle writebacks / updates
            std::cout << "\nStep 4) Controllers handle writebacks" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 4) Controllers handle writebacks", WHITE);
            console.log("----------------------------------------", WHITE);

            for (int i = 0; i < NUM_NODES-1; i++) {
                nodes[arbOrder[i]].cc.step(bus, false);
            }

            std::cout << "\nStep 5) Source snoops" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 5) Source snoops", WHITE);
            console.log("----------------------------------------", WHITE);

            nodes[arbOrder[NUM_NODES-1]].cc.snoopBus(bus, bus.current(), true);
            bus.clear();

            std::cout << "\nStep 6) Source responds" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 6) Source responds", WHITE);
            console.log("----------------------------------------", WHITE);
            
            nodes[arbOrder[NUM_NODES-1]].cc.step(bus, true);

            std::cout << "\nStep 7) Snoop in case of BUS_UPD" << std::endl << std::endl;
            console.log("----------------------------------------", WHITE);
            console.log("Step 7) Snoop in case of BUS_UPD", WHITE);
            console.log("----------------------------------------", WHITE);
            
            for (int i = 0; i < NUM_NODES-1; i++) {
                nodes[arbOrder[i]].cc.snoopBus(
                    bus, bus.current(), false
                );
            }

            // 8) Clear bus (end of cycle)
            bus.clear();
        }

        console.update(instructionsAdded, commands);

        //fill processors with given instructions
        if(!commands.empty()) {
            std::cout << "filling in commands" << std::endl;
            instructionsAdded = true;

            for (auto& c : commands) {
                if (c.ReadWrite)
                    nodes[c.nodeId].cpu.issueRead(c.addr);
                else
                    nodes[c.nodeId].cpu.issueWrite(c.addr);

                // Fill memory with used addresses
                memory.fillMemory(c.addr);
            }

            commands.clear();
        }

        // ------------------------------------
        // DRAWING
        // ------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Cycle: %d  (SPACE = step)", cycle),
                 20, 20, 20, BLACK);



        for (int i = 0; i < NUM_NODES; i++) {
            float y = layout.baseY + i * layout.rowSpacing;

            DrawProcessor(i, {layout.xStartingPointProc, y},
                        layout.procWidth, layout.procHeight);

            DrawCache({layout.xStartingPointCache, y - 20},
                    layout.cacheWidth, layout.cacheHeight);

            DrawCoherenceController({layout.xStartingPointCC, y - 40},
                                    layout.ccWidth, layout.ccHeight);

            // Connections
            DrawLine(layout.xStartingPointProc + layout.procWidth, y + layout.procHeight / 2,
                    layout.xStartingPointCache, y + layout.procHeight / 2, BLACK);

            DrawLine(layout.xStartingPointCache + layout.cacheWidth, y + layout.procHeight / 2,
                    layout.xStartingPointCC, y + layout.procHeight / 2, BLACK);

            DrawLine(layout.xStartingPointCC + layout.ccWidth, y - 40 + layout.ccHeight / 2,
                    layout.xStartingPointCC + layout.ccWidth + layout.distanceCCtoVericalLine, y - 40 + layout.ccHeight / 2, BLACK);
        }

        DrawBus(layout.xStartingPointBus, layout.yPositionBus, layout.busWidth, layout.distanceLabelsBus, layout.distanceLinesBus);
        DrawMemory({layout.xStartingPointMemory, layout.yPositionMemoryMedian}, layout.memWidth, layout.memHeight);      

        DrawDragonProcessorFSM({layout.xFSMPr, layout.yFSMPr});
        DrawDragonBusFSM({layout.xFSMBus, layout.yFSMBus});

        // vertical line that connects Coherence Controllers with Bus lines
        DrawLine(layout.xStartingPointCC + layout.ccWidth + layout.distanceCCtoVericalLine, layout.baseY + layout.procHeight / 2,
                 layout.xStartingPointCC + layout.ccWidth + layout.distanceCCtoVericalLine, layout.yPositionBus + layout.distanceLinesBus*3, BLACK);

        
        // Console area
        console.drawLogger(1800, 1800, layout.consoleLoggerX);
        console.drawCmdPrompt(1800, layout.consoleLoggerX, layout.consoleCmdPromptY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
