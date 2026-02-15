#pragma once
#include "raylib.h"
#include "cache.hpp"

constexpr Color HIGHLIGHT = YELLOW;
constexpr float HL_THICKNESS = 3.0f;

//VISUALIZATION---------------------------------------------------------------------------------------------------

struct VisualLayout {
    //Frame
    float baseY = 100;

    float xStartingPointProc = 50;
    float xStartingPointCache = 150;
    float xStartingPointCC = 350;

    //dragonFSM proc top right corner
    float xFSMPr = 800;
    float yFSMPr = 100;

    //dragonFSM bus top right corner
    float xFSMBus = 800;
    float yFSMBus = 500;

    //distance cc to vertical line
    float distanceCCtoVericalLine = 30;

    // Processor
    float procWidth  = 70;
    float procHeight = 40;

    // Cache
    float cacheWidth  = 160;
    float cacheHeight = 80;
    float cacheTagWidth  = 60;
    float cacheDataWidth = 60;

    // Coherence controller
    float ccWidth  = 200;
    float ccHeight = 120;

    // Bus
    float xStartingPointBus = 50;
    float yPositionBus = 800;
    float busWidth = 800;
    float distanceLabelsBus = 50;
    float distanceLinesBus = 15;

    // Memory
    float xStartingPointMemory = busWidth+xStartingPointBus;
    float yPositionMemoryMedian = yPositionBus;
    float memWidth  = 160;
    float memHeight = 60;

    // Spacing
    float rowSpacing = 180;

    // Console
    float consoleLoggerX = 1200;
    float consoleCmdPromptY = 900;
};

void DrawProcessor(int id, Vector2 pos, float width, float height);
//void DrawCache(int id, Cache& cache, Vector2 pos);
void DrawCache(Vector2 pos, float width, float height);
void DrawCoherenceController(Vector2 pos, float width, float height);
void DrawBus(float x, float y, float width, float distanceLabels, float distanceLines);
void DrawMemory(Vector2 pos, float width, float height);

//ANIMATION---------------------------------------------------------------------------------------------------

enum class VisualStage {
    Idle,
    ProcActive,
    CacheActive,
    HitResult,
    CCActive
};

struct VisualAnimation {
    VisualStage stage = VisualStage::Idle;
    int activeNode = -1;
    bool isMiss = false;
};

void AdvanceVisualStage(VisualAnimation& vis);
