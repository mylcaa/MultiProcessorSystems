#pragma once
#include "common.hpp"
#include "raylib.h"
#include <string>
#include <vector>

struct ConsoleLine {
    std::string text;
    Color color;
};

class Console {
public:
    void log(const std::string& text, Color color = BLACK);
    void update(bool simulation_started, std::vector<Cmd>& commands);
    void drawLogger(int screenWidth, int screenHeight, int consoleX);
    void drawCmdPrompt(int screenHeight, int loggerX, int consoleY);
    void parseInput(const std::string& input, std::vector<Cmd>& commands);

private:
    std::vector<ConsoleLine> lines;
    std::vector<std::string> cmd;

    std::string inputBuffer;
    std::string submittedCommand;

    int historyIndexLines = 0;
    int historyIndexCmd = 0;
};
