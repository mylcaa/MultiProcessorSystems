#include "draw_dragon_fsm.hpp"
#include <math.h>

#define circleRadius 30
#define stateDistance 240
#define sizeArrow 12

static void DrawState(Vector2 p, const char* name) {
    DrawCircleV(p, circleRadius, YELLOW);
    DrawCircleLines(p.x, p.y, circleRadius, BLACK);
    DrawText(name, p.x - 6, p.y - 7, 14, BLACK);
}

static void DrawArrow(Vector2 a, Vector2 b, const char* label, Vector2 textOffset) {
    DrawLineV(a, b, BLACK);
    DrawText(label, textOffset.x, textOffset.y, 10, BLACK);
}

void DrawDragonProcessorFSM(Vector2 o) {
    Vector2 E  = { o.x + 0,   o.y + 0 };
    Vector2 Sc = { o.x + stateDistance, o.y + 0 };
    Vector2 Sm = { o.x + 0,   o.y + stateDistance };
    Vector2 M  = { o.x + stateDistance, o.y + stateDistance };

    DrawText("Processor-side Dragon FSM", o.x + 20, o.y - 80, 14, BLACK);

    DrawState(E,  "E");
    DrawState(Sc, "Sc");
    DrawState(Sm, "Sm");
    DrawState(M,  "M");

    //arrows from nowhere
    DrawArrow({Sc.x + stateDistance/2, Sc.y},  {Sc.x + circleRadius, Sc.y}, "PrRdMiss/BusRd(C)",  {Sc.x + 40,  Sc.y - 15});
    DrawDirectionalTriangle({Sc.x + circleRadius, Sc.y}, sizeArrow, ArrowDir::Left, BLACK);
    DrawArrow({E.x - stateDistance/2, E.y},  {E.x - circleRadius, E.y}, "PrRdMiss/BusRd(!C)",  {E.x - stateDistance/2,  E.y - 15});
    DrawDirectionalTriangle({E.x - circleRadius, E.y}, sizeArrow, ArrowDir::Right, BLACK);

    DrawArrow({M.x + stateDistance/2, M.y},  {M.x + circleRadius, M.y}, "PrWrMiss/BusRd(!C)",  {M.x + 40,  M.y - 15});
    DrawDirectionalTriangle({M.x + circleRadius, M.y}, sizeArrow, ArrowDir::Left, BLACK);
    DrawArrow({Sm.x - stateDistance/2, Sm.y},  {Sm.x - circleRadius, Sm.y}, "PrWrMiss/\nBusRd(C);BusUpd",  {Sm.x - stateDistance/2,  Sm.y - 30});
    DrawDirectionalTriangle({Sm.x - circleRadius, Sm.y}, sizeArrow, ArrowDir::Right, BLACK);

    //draw arrows from one state to another
    //Sc -> M
    DrawArrow({Sc.x, Sc.y + circleRadius}, {M.x, M.y - circleRadius},  "PrWr/BusUpd(!C)",   {Sc.x + 10, Sc.y + 40});
    DrawDirectionalTriangle({M.x, M.y - circleRadius}, sizeArrow, ArrowDir::Down, BLACK);
    //Sm -> M
    DrawArrow({Sm.x + circleRadius, Sm.y}, {M.x - circleRadius, M.y},  "PrWr/BusUpd(!C)",   {Sm.x + 20,  Sm.y + 30});
    DrawDirectionalTriangle({M.x - circleRadius, M.y}, sizeArrow, ArrowDir::Right, BLACK);
    //E -> M
    Vector2 p1 = {M.x - circleRadius*cosf(PI/4), M.y - circleRadius*cosf(PI/4)};
    DrawArrow({E.x + circleRadius*cosf(PI/4), E.y + circleRadius*cosf(PI/4)}, {M.x - circleRadius*cosf(PI/4), M.y - circleRadius*cosf(PI/4)},  "PrWr/-",   {M.x - circleRadius*cosf(PI/4) - 30, M.y - circleRadius*cosf(PI/4) - 20});
    DrawTriangleLines(p1, {p1.x, p1.y - sizeArrow}, {p1.x - sizeArrow, p1.y}, BLACK);

    //Sc -> Sm
    p1 = {Sm.x + circleRadius*cosf(PI/4), Sm.y - circleRadius*cosf(PI/4)};
    DrawArrow({Sc.x - circleRadius*cosf(PI/4), Sc.y + circleRadius*cosf(PI/4)}, {Sm.x + circleRadius*cosf(PI/4), Sm.y - circleRadius*cosf(PI/4)},  "PrWr/BusUpd(C)",   {Sm.x + circleRadius*cosf(PI/4) - 40, Sm.y - circleRadius*cosf(PI/4) - 20});
    DrawTriangleLines(p1, {p1.x + sizeArrow, p1.y}, {p1.x, p1.y - sizeArrow}, BLACK);

    //draw arrow from state to same state
    DrawArcArrow({E.x - circleRadius, E.y}, {E.x, E.y - circleRadius}, -80.0f, 1.0f, 12.0f, BLACK, "PrRd/-");
    DrawDirectionalTriangle({E.x, E.y - circleRadius}, sizeArrow, ArrowDir::Down, BLACK);
    
    DrawArcArrow({Sm.x, Sm.y + circleRadius}, {Sm.x - circleRadius, Sm.y}, -80.0f, 1.0f, 12.0f, BLACK, "PrRd/-\nPrWr/BusUpd(C)");
    DrawDirectionalTriangle({Sm.x - circleRadius, Sm.y}, sizeArrow, ArrowDir::Right, BLACK);
    
    DrawArcArrow({Sc.x + circleRadius, Sc.y}, {Sc.x, Sc.y - circleRadius},  80.0f, 1.0f, 12.0f, BLACK, "PrRd/-");
    DrawDirectionalTriangle({Sc.x, Sc.y - circleRadius}, sizeArrow, ArrowDir::Down, BLACK);
    
    DrawArcArrow({M.x, M.y + circleRadius}, {M.x + circleRadius, M.y}, 80.0f, 1.0f, 12.0f, BLACK, "PrRd/-\nPrWr/-");
    DrawDirectionalTriangle({M.x + circleRadius, M.y}, sizeArrow, ArrowDir::Left, BLACK);
}

void DrawDragonBusFSM(Vector2 o) {
    Vector2 E  = { o.x + 0,   o.y + 0 };
    Vector2 Sc = { o.x + stateDistance, o.y + 0 };
    Vector2 Sm = { o.x + 0,   o.y + stateDistance };
    Vector2 M  = { o.x + stateDistance, o.y + stateDistance };

    DrawText("Bus-side Dragon FSM", o.x + 50, o.y - 40, 14, BLACK);

    DrawState(E,  "E");
    DrawState(Sc, "Sc");
    DrawState(Sm, "Sm");
    DrawState(M,  "M");

    //draw arrows from one state to another
    //E -> Sc
    DrawArrow({E.x + circleRadius, E.y}, {Sc.x - circleRadius, Sc.y},  "BusRd/-",   {E.x + stateDistance/2, E.y - 15});
    DrawDirectionalTriangle({Sc.x - circleRadius, Sc.y}, sizeArrow, ArrowDir::Right, BLACK);
    //M -> Sm
    DrawArrow({M.x - circleRadius, M.y}, {Sm.x + circleRadius, Sm.y},  "BusRd/FLush",   {Sm.x + stateDistance/2,  Sm.y + 15});
    DrawDirectionalTriangle({Sm.x + circleRadius, Sm.y}, sizeArrow, ArrowDir::Left, BLACK);
    //Sm -> Sc
    Vector2 p1 = {Sc.x - circleRadius*cosf(PI/4), Sc.y + circleRadius*cosf(PI/4)};
    DrawArrow({Sm.x + circleRadius*cosf(PI/4), Sm.y - circleRadius*cosf(PI/4)}, {Sc.x - circleRadius*cosf(PI/4), Sc.y + circleRadius*cosf(PI/4)},  "BusUpd/Update",   {Sm.x + stateDistance/2, Sm.y - stateDistance/2});
    DrawTriangleLines(p1, {p1.x, p1.y  + sizeArrow}, {p1.x - sizeArrow, p1.y}, BLACK);


    //draw arrow from state to same state
    DrawArcArrow({Sm.x, Sm.y + circleRadius}, {Sm.x - circleRadius, Sm.y}, -80.0f, 1.0f, 12.0f, BLACK, "BusRd/Flush");
    DrawDirectionalTriangle({Sm.x - circleRadius, Sm.y}, sizeArrow, ArrowDir::Right, BLACK);
    DrawArcArrow({Sc.x + circleRadius, Sc.y}, {Sc.x, Sc.y - circleRadius},  80.0f, 1.0f, 12.0f, BLACK, "BusRd/-\nBusUpd/Update");
    DrawDirectionalTriangle({Sc.x, Sc.y - circleRadius}, sizeArrow, ArrowDir::Down, BLACK);
}

void DrawDirectionalTriangle(Vector2 tip,
                             float size,
                             ArrowDir dir,
                             Color color)
{
    float halfBase = size * 0.6f;

    Vector2 p1 = tip;
    Vector2 p2, p3;

    switch (dir) {
        // works
        case ArrowDir::Up:
            p2 = { tip.x - halfBase, tip.y + size };
            p3 = { tip.x + halfBase, tip.y + size };
            break;

        case ArrowDir::Down:
            p2 = { tip.x + halfBase, tip.y - size };
            p3 = { tip.x - halfBase, tip.y - size };
            break;

        case ArrowDir::Left:
            p2 = { tip.x + size, tip.y + halfBase };
            p3 = { tip.x + size, tip.y - halfBase };
            break;
        // works
        case ArrowDir::Right:
            p2 = { tip.x - size, tip.y - halfBase };
            p3 = { tip.x - size, tip.y + halfBase };
            break;
    }

    DrawTriangleLines(p1, p2, p3, color);
}

// Draw an arced arrow from start → end
// curvature > 0  : arc bends left
// curvature < 0  : arc bends right
void DrawArcArrow(Vector2 start,
                  Vector2 end,
                  float curvature,
                  float thickness,
                  float arrowSize,
                  Color color,
                  const char* label)
{
    const int SEGMENTS = 32;

    // Midpoint
    Vector2 mid = {
        (start.x + end.x) * 0.5f,
        (start.y + end.y) * 0.5f
    };

    // Direction and perpendicular
    Vector2 dir = { end.x - start.x, end.y - start.y };
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len < 1.0f) return;

    dir.x /= len;
    dir.y /= len;

    Vector2 perp = { -dir.y, dir.x };

    // Control point (quadratic Bézier)
    Vector2 control = {
        mid.x + perp.x * curvature,
        mid.y + perp.y * curvature
    };

    // Draw Bézier arc
    Vector2 prev = start;
    for (int i = 1; i <= SEGMENTS; i++) {
        float t = i / (float)SEGMENTS;

        // Quadratic Bézier formula
        Vector2 p = {
            (1 - t) * (1 - t) * start.x +
            2 * (1 - t) * t * control.x +
            t * t * end.x,

            (1 - t) * (1 - t) * start.y +
            2 * (1 - t) * t * control.y +
            t * t * end.y
        };

        DrawLineEx(prev, p, thickness, color);
        prev = p;
    }

    // Tangent direction near the end
    float t = 0.97f;
    Vector2 tangent = {
        2 * (1 - t) * (control.x - start.x) +
        2 * t * (end.x - control.x),

        2 * (1 - t) * (control.y - start.y) +
        2 * t * (end.y - control.y)
    };

    float tlen = sqrtf(tangent.x * tangent.x + tangent.y * tangent.y);
    if (tlen > 0.001f) {
        tangent.x /= tlen;
        tangent.y /= tlen;
    }

    Vector2 perpT = { -tangent.y, tangent.x };

    // Arrowhead
    Vector2 tip = end;
    Vector2 left = {
        tip.x - tangent.x * arrowSize + perpT.x * (arrowSize * 0.5f),
        tip.y - tangent.y * arrowSize + perpT.y * (arrowSize * 0.5f)
    };
    Vector2 right = {
        tip.x - tangent.x * arrowSize - perpT.x * (arrowSize * 0.5f),
        tip.y - tangent.y * arrowSize - perpT.y * (arrowSize * 0.5f)
    };

    DrawTriangle(tip, left, right, color);

    // Optional label
    if (label) {
        Vector2 labelPos = {
            mid.x + perp.x * (curvature * 0.5f),
            mid.y + perp.y * (curvature * 0.5f)
        };
        DrawText(label, labelPos.x, labelPos.y, 10, BLACK);
    }
}



