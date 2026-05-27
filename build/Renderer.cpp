#include "Renderer.h"

Renderer::Renderer()
{
    mFramebuffer = TGAImage(mWidth, mHeight, TGAImage::RGB);
}

Vector3D Renderer::ViewportTransform(Vector3D vector)
{
	return Vector3D((vector.x + 1.) * mWidth / 2,
		(vector.y + 1.) * mHeight / 2);
}

void Renderer::DrawLine(int ax, int ay, int bx, int by, const TGAColor color)
{
    bool steepLine = std::abs(ax - bx) < std::abs(ay - by);

    if (steepLine)//if its a steep line, transpose the image
    {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx) //make it left to right
    {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    int y = ay;
    float error = 0.0;
    for (int x = ax; x <= bx; x++)
    {
        int px = steepLine ? y : x;
        int py = steepLine ? x : y;

        mFramebuffer.set(px, py, color);

        error += 2 * std::abs(by - ay);
        y += (by > ay ? 1 : -1) * (error > bx - ax);
        error -= 2 * (bx - ax) * (error > bx - ax);
    }
}

void Renderer::DrawTriangle(int ax, int ay, int bx, int by, int cx, int cy, TGAColor color)
{
    DrawLine(ax, ay, bx, by, color);
    DrawLine(bx, by, cx, cy, color);
    DrawLine(cx, cy, ax, ay, color);
}

void Renderer::WriteFramebufferToFile()
{
    mFramebuffer.write_tga_file("framebuffer.tga");
}
