#include "Renderer.h"
#include <algorithm>

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

void Renderer::DrawBoundingBox(int ax, int ay, int bx, int by, int cx, int cy, TGAColor color) {
    int bbminx = std::min(std::min(ax, bx), cx);
    int bbminy = std::min(std::min(ay, by), cy);
    int bbmaxx = std::max(std::max(ax, bx), cx);
    int bbmaxy = std::max(std::max(ay, by), cy);

#pragma omp parallel for 
    //Embarrassingly Parallel method
    for (int x = bbminx; x <= bbmaxx; x++)
    {
        for (int y = bbminy; y <= bbmaxy; y++)
        {
            mFramebuffer.set(x, y, color);
        }
    }

}

void Renderer::DrawTriangleFilled(int ax, int ay, int bx, int by, int cx, int cy, TGAColor color) {
    //As there are only 3 elements, we can do bubble sorting
    if (ay > by) { std::swap(ax, bx); std::swap(ay, by); }
    if (ay > cy) { std::swap(ax, cx); std::swap(ay, cy); }
    if (by > cy) { std::swap(bx, cx); std::swap(by, cy); }
    int totalHeight = cy - ay;
    //This bubble sorting will make ay be the lowest point and cy the highest
    
    //check if botttom half is not degenerate (two vertex with the same Y position breaks the iteration)
    if (ay != by)
    {
        int segmentHeight = by - ay;
        for (int y = ay; y <= by; y++) //sweeping horizontal line from ay to by
        {
            int x1 = ax + ((cx - ax) * (y - ay)) / totalHeight; //x position following the long edge
            int x2 = ax + ((bx - ax) * (y - ay)) / segmentHeight; //x position following the short/segment edge

            //Dmitry's implementation inlines the step calculation, and reorganize so that division is done last.
            //he does that to keep everything working with Int values.

            for (int x = std::min(x1, x2); x < std::max(x1, x2); x++)//horizontal line
            {
                mFramebuffer.set(x, y, color);
            }
        }
    }
    if (by != cy)//same for the upper half
    {
        int segmentHeight = cy - by;
        for (int y = by; y < cy; y++)//from the middle to the top by -> cy
        {
            int x1 = ax + ((cx - ax) * (y - ay)) / totalHeight;
            int x2 = bx + ((cx - bx) * (y - by)) / segmentHeight;
            for (int x = std::min(x1, x2); x < std::max(x1, x2); x++)
            {
                mFramebuffer.set(x, y, color);
            }
        }
    }
}

void Renderer::WriteFramebufferToFile()
{
    mFramebuffer.write_tga_file("../build/framebuffer.tga");
}
