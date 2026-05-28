#pragma once
#include "../tgaimage.h"
#include "Geometry.h"

namespace Colors {
    constexpr TGAColor white = { 255, 255, 255, 255 }; // attention, BGRA order
    constexpr TGAColor green = { 0, 255,   0, 255 };
    constexpr TGAColor red = { 0,   0, 255, 255 };
    constexpr TGAColor blue = { 255, 128,  64, 255 };
    constexpr TGAColor yellow = { 0, 200, 255, 255 };
}

class Renderer {
private:
    const int mWidth = 128;
    const int mHeight = 128;

    TGAImage mFramebuffer;
public:
    Renderer();

    Vector3D ViewportTransform(Vector3D vector);
    void DrawLine(int ax, int ay, int bx, int by, const TGAColor color);
    void DrawTriangle(int ax, int ay, int bx, int by, int cx, int cy, TGAColor color);

    void WriteFramebufferToFile();
};

