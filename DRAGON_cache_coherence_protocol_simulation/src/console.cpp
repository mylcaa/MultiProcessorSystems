#include "console.hpp"
#include <algorithm>

void Console::log(const std::string& text, Color color) {
    lines.push_back({text, color});
    if (lines.size() > 500) {
        lines.erase(lines.begin());
    }
}

void Console::update(bool simulation_started, std::vector<Cmd>& commands) {

    if(simulation_started) {
        // History UP
        if (IsKeyPressed(KEY_UP) && !lines.empty()) {
            if (historyIndexLines > 0){
                historyIndexLines--;
            }
        }

        // History DOWN
        if (IsKeyPressed(KEY_DOWN) && !lines.empty()) {
            if (historyIndexLines < ((int)lines.size() - 1)){
                historyIndexLines++;
            }
        }
    } else {
        // Character input
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125) {
                inputBuffer += (char)key;
            }
            key = GetCharPressed();
        }

        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !inputBuffer.empty()) {
            inputBuffer.pop_back();
        }

        // Submit
        if (IsKeyPressed(KEY_ENTER)) {
            parseInput(inputBuffer, commands);
            inputBuffer.clear();
        }
    }
}

void Console::drawLogger(int screenWidth, int screenHeight, int consoleX) {

    int consoleHeight = screenHeight;
    int consoleY = 0;

    int consoleWidth = screenWidth - consoleX;

    DrawLine(consoleX - 5, 0, consoleX - 5, screenHeight, BLACK);
    DrawLine(consoleX, 0, consoleX, screenHeight, BLACK);

    DrawRectangle(consoleX, consoleY, consoleWidth, consoleHeight, Fade(BLACK, 0.95f));
    DrawText("SIMULATED PROTOCOL LOGGER OUTPUT:", consoleX + 10, consoleY + 10, 16, WHITE);

    int lineHeight = 18;
    int maxVisible = (consoleHeight - 40) / lineHeight;

    for (int i = 0; i < maxVisible && (historyIndexLines + i) < (int)lines.size(); i++) {
        DrawText(
            ("> " + lines[historyIndexLines + i].text).c_str(),
            consoleX + 10,
            consoleY + 10 + (i+1) * lineHeight,
            20,
            lines[historyIndexLines + i].color
        );
    }
}

void Console::drawCmdPrompt(int screenHeight, int loggerX, int consoleY) {
    int consoleWidth = loggerX;
    int consoleX = 0;

    int consoleHeight = screenHeight - consoleY;

    DrawLine(0, consoleY - 5, consoleWidth, consoleY - 5, BLACK);
    DrawLine(0, consoleY, consoleWidth, consoleY, BLACK);

    DrawRectangle(consoleX, consoleY, consoleWidth, consoleHeight, Fade(BLACK, 0.95f));
    DrawText("INPUT CPU INSTRUCTIONS AS ARRAY IN FORMAT: P0...3 R/W ADDR ; P0...3 R/W ADDR ; ...", consoleX + 10, consoleY + 10, 25, WHITE);

    int lineHeight = 25;

    DrawText(
            ("> " + inputBuffer).c_str(),
            consoleX + 10,
            consoleY + 10 + (cmd.size() + 1) * lineHeight,
            25,
            WHITE
    );
}

void Console::parseInput(const std::string& input,
                               std::vector<Cmd>& commands)
{
    commands.clear();   // reset previous commands

    std::stringstream ss(input);
    std::string command;

    // Split by ';'
    while (std::getline(ss, command, ';')) {

        // Trim whitespace
        command.erase(0, command.find_first_not_of(" \t\n\r"));
        command.erase(command.find_last_not_of(" \t\n\r") + 1);


        if (command.empty()) continue;

        std::stringstream cmdStream(command);

        std::string processorStr;
        char operation;
        std::string addressStr;

        cmdStream >> processorStr >> operation >> addressStr;

        int nodeId = std::stoi(processorStr.substr(1));

        // Basic validation
        if ((nodeId >= NUM_NODES || processorStr[0] != 'P') || (operation != 'R' && operation != 'W')) {
            std::cout << "first condition: " << (nodeId >= NUM_NODES || processorStr[0] != 'P') << std::endl;
            std::cout << "second condition: " << (operation != 'R' && operation != 'W') << std::endl;
            std::cout << "Invalid processor format in: " << command << std::endl;
            continue;
        }

        int address = 0;
        if (addressStr.find("0x") == 0 || addressStr.find("0X") == 0)
            address = std::stoi(addressStr, nullptr, 16);
        else
            address = std::stoi(addressStr);

        Cmd newCmd;
        newCmd.nodeId = nodeId;
        newCmd.ReadWrite = (operation == 'R');
        newCmd.addr = address;

        commands.push_back(newCmd);
    }
}
