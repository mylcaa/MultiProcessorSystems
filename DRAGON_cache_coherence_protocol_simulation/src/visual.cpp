#include "visual.hpp"

void DrawProcessor(int id, Vector2 pos, float width, float height) {
    DrawRectangle(pos.x, pos.y, width, height, GREEN);
    DrawText(TextFormat("P%d", id),
             pos.x + width * 0.35f,
             pos.y + height * 0.3f,
             16,
             WHITE);
}

void DrawCache(Vector2 pos, float width, float height) {
    DrawRectangle(pos.x, pos.y, width, height, BLUE);
    DrawText("Cache", pos.x + width * 0.35f, pos.y - 15, 14, WHITE);

    float subY = pos.y + 20;
    float subH = height - 30;

    DrawRectangleLines(pos.x + 10, subY, 60, subH, WHITE);
    DrawText("Tag", pos.x + 25, subY + subH / 2 - 6, 12, WHITE);

    DrawRectangleLines(pos.x + width - 70, subY, 60, subH, WHITE);
    DrawText("Data", pos.x + width - 55, subY + subH / 2 - 6, 12, WHITE);
}

void DrawCoherenceController(Vector2 pos, float width, float height) {
    DrawRectangleLines(pos.x, pos.y, width, height, BLACK);
    DrawText("Coherence Controller", pos.x + 20, pos.y - 15, 14, BLACK);

    // FSM
    DrawText("FSM", pos.x + 10, pos.y + 10, 12, BLACK);
    DrawCircle(pos.x + 40, pos.y + 40, 12, YELLOW);

    // Snooper
    DrawText("Snooper", pos.x + width * 0.5f, pos.y + 20, 12, BLACK);
    DrawRectangle(pos.x + width * 0.45f, pos.y + 40, 80, 20, LIGHTGRAY);

    // OTT
    DrawText("OTT", pos.x + 10, pos.y + height - 40, 12, BLACK);
    DrawRectangle(pos.x + 40, pos.y + height - 45, 50, 30, ORANGE);

    // WB
    DrawText("WB", pos.x + width - 80, pos.y + height - 40, 12, BLACK);
    DrawRectangle(pos.x + width - 50, pos.y + height - 45, 30, 30, RED);
}

void DrawBus(float x, float y, float width, float distanceLabels, float distanceLines) {

    DrawLine(x + distanceLabels, y, x + width, y, BLACK);
    DrawLine(x + distanceLabels, y + distanceLines, x + width, y + distanceLines, BLACK);
    DrawLine(x + distanceLabels, y + 2*distanceLines, x + width, y + 2*distanceLines, BLACK);
    DrawLine(x + distanceLabels, y + 3*distanceLines, x + width, y + 3*distanceLines, BLACK);

    DrawText("CMD", x, y - 2, 10, BLACK);
    DrawText("ADDR", x, y + distanceLines - 2, 10, BLACK);
    DrawText("DATA", x, y + 2*distanceLines - 4, 10, BLACK);
    DrawText("COPIES\nEXIST", x, y + 3*distanceLines - 6, 10, BLACK);
}

void DrawMemory(Vector2 pos, float width, float height) {
    DrawRectangle(pos.x, pos.y, width, height, BLUE);
    DrawText("Main Memory",
             pos.x + width * 0.15f,
             pos.y + height * 0.35f,
             14,
             WHITE);
}

void AdvanceVisualStage(VisualAnimation& vis)
{
    switch (vis.stage) {
        case VisualStage::Idle:
            vis.stage = VisualStage::ProcActive;
            break;

        case VisualStage::ProcActive:
            vis.stage = VisualStage::CacheActive;
            break;

        case VisualStage::CacheActive:
            vis.stage = vis.isMiss ? VisualStage::CCActive
                                   : VisualStage::HitResult;
            break;

        case VisualStage::HitResult:
        case VisualStage::CCActive:
            vis.stage = VisualStage::Idle;
            vis.activeNode = -1;
            break;
    }
}


