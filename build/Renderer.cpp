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

void Renderer::DrawTriangleFilled(int ax, int ay, int bx, int by, int cx, int cy, TGAColor color) {
    //Making an array of Vector2D using the vertices I'm passing
    Vector2D Verts[3] = { Vector2D(ax, ay), Vector2D(bx, by), Vector2D(cx, cy) };
    
    //Sorting them so that Verts[0] is has the highest y value.
    std::sort(Verts, Verts + 3, [](Vector2D a, Vector2D b) { return a.y > b.y; });

    //Here I have to draw/fill the triangle in two parts beause
    //triangles have 3 edges, and none of them goes from the top straight
    //to the base both sides at the same time. The vertex at the middle will break
    //one of the sides. That changes the pattern we were following when
    //respecting the distance between edges.

    //Part 1 - Iterate through each y value until we reach the base.
    for (int y = Verts[0].y; y >= Verts[1].y; y--)
    {
        //Find the step to increment when getting the start and the end
        //positions of for the lines that will make up the triangle's face.
        //We'll always have a long edge and a short edge, the short is the one
        //broken by a vertex.

        //The long edge goes from the highest Y vertex towards the lowest (Vert[2].y)
        //So in order to know the amount of displacement in the X axis, we need to calculate it
        //by divinding the current distance between the iterated Y value and the highest Y value by
        //the distance between the top and the bottom.
        float stepLong = (float)(y - Verts[0].y) / (float)(Verts[2].y - Verts[0].y);
        //short edge goes from the highest Y vertex towards the middle vertex.
        float stepShort = (float)(y - Verts[0].y) / (float)(Verts[1].y - Verts[0].y);

        //Now we get the start position of the scan/fill line by adding the
        //calculated step multiplied by the distance between the x position of the highest vertex and the next one.
        float fillStart = Verts[0].x + stepShort * (Verts[1].x - Verts[0].x);

        //the end of the line will use the step calculated for the long edge, multiplied by the X distance from the
        //highest vertex to the bottom one.
        float fillEnd = Verts[0].x + stepLong * (Verts[2].x - Verts[0].x);

        //if it turns out the the starting value is bigger then the end value, it means our triangle is mirrored.
        //The long edge is on the left instead of the right, so we swap
        if (fillStart > fillEnd) std::swap(fillStart, fillEnd);

        DrawLine((int)fillStart, y, (int)fillEnd, y, color);
    }

    //For the lower half of the triangle I repeat the same steps as above
    //but I'll start from the middle vertex and go to the bottom.
    for (int y = Verts[1].y; y >= Verts[2].y; y--)
    {
        //stepLong still calculates from top to bottom     Bottom  <-----  Top
        float stepLong = (float)(y - Verts[0].y) / (float)(Verts[2].y - Verts[0].y);

        //stepShort calculates from middle to bottom       Bottom  <-----  Middle
        float stepShort = (float)(y - Verts[1].y) / (float)(Verts[2].y - Verts[1].y);

        //fillStart position will increment from the middle X position towards the bottom x position.
        float fillStart = Verts[1].x + stepShort * (Verts[2].x - Verts[1].x);
        //fillEnd position will increment from the Top X position towards the bottom x position.
        float fillEnd = Verts[0].x + stepLong * (Verts[2].x - Verts[0].x);

        //Once again, we check if we are starting from the left and going to right, and swap if that's the case.
        if (fillStart > fillEnd) std::swap(fillStart, fillEnd);

        DrawLine((int)fillStart, y, (int)fillEnd, y, color);
    }
}

void Renderer::WriteFramebufferToFile()
{
    mFramebuffer.write_tga_file("../build/framebuffer.tga");
}
