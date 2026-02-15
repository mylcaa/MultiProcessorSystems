#pragma once
#include <raylib.h>

enum class ArrowDir {
    Up,
    Down,
    Left,
    Right
};

void DrawDragonProcessorFSM(Vector2 origin);
void DrawDragonBusFSM(Vector2 origin);
void DrawDirectionalTriangle(Vector2 tip, float size, ArrowDir dir, Color color);
// Draw an arced arrow from start → end
// curvature > 0  : arc bends left
// curvature < 0  : arc bends right
void DrawArcArrow(Vector2 start,Vector2 end, float curvature, float thickness, float arrowSize, Color color, const char* label);
