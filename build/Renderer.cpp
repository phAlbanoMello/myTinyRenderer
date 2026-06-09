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
    //As there are only 3 elements, we can do bubble sorting
    //in practice it seems it doesn't make much difference from using std::sort at the entire array, because its
    //only 3 elements. But I'm changing this at this commit to experiment with the instructor's solution.
    if (ay > by) { std::swap(ax, bx); std::swap(ay, by); }
    if (ay > cy) { std::swap(ax, cx); std::swap(ay, cy); }
    if (by > cy) { std::swap(bx, cx); std::swap(by, cy); }
    int totalHeight = cy - ay;
    //This bubble sorting will make ay be the lowest point and cy the highest


    //Dmitry's solution
    //Dmitry's solution is better then the one I've used before because
    //1 - It checks if any vertex has the same Y value to avoid division by 0
    //2 - It uses only int math, being lighter on the CPU
    
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

    //My Solution

    ////Making an array of Vector2D using the vertices I'm passing
    //Vector2D Verts[3] = { Vector2D(cx, cy), Vector2D(bx, by), Vector2D(ax, ay) };

    ////Here I have to draw/fill the triangle in two parts beause
    ////triangles have 3 edges, and none of them goes from the top straight
    ////to the base both sides at the same time. The vertex at the middle will break
    ////one of the sides. That changes the pattern we were following when
    ////respecting the distance between edges.

    ////Part 1 - Iterate through each y value until we reach the base.
    //for (int y = Verts[0].y; y >= Verts[1].y; y--)
    //{
    //    //Find the step to increment when getting the start and the end
    //    //positions of for the lines that will make up the triangle's face.
    //    //We'll always have a long edge and a short edge, the short is the one
    //    //broken by a vertex.

    //    //The long edge goes from the highest Y vertex towards the lowest (Vert[2].y)
    //    //So in order to know the amount of displacement in the X axis, we need to calculate it
    //    //by divinding the current distance between the iterated Y value and the highest Y value by
    //    //the distance between the top and the bottom.
    //    float stepLong = (float)(y - Verts[0].y) / (float)(Verts[2].y - Verts[0].y);
    //    //short edge goes from the highest Y vertex towards the middle vertex.
    //    float stepShort = (float)(y - Verts[0].y) / (float)(Verts[1].y - Verts[0].y);

    //    //Now we get the start position of the scan/fill line by adding the
    //    //calculated step multiplied by the distance between the x position of the highest vertex and the next one.
    //    float fillStart = Verts[0].x + stepShort * (Verts[1].x - Verts[0].x);

    //    //the end of the line will use the step calculated for the long edge, multiplied by the X distance from the
    //    //highest vertex to the bottom one.
    //    float fillEnd = Verts[0].x + stepLong * (Verts[2].x - Verts[0].x);

    //    //if it turns out the the starting value is bigger then the end value, it means our triangle is mirrored.
    //    //The long edge is on the left instead of the right, so we swap
    //    if (fillStart > fillEnd) std::swap(fillStart, fillEnd);

    //    DrawLine((int)fillStart, y, (int)fillEnd, y, color);
    //}

    ////For the lower half of the triangle I repeat the same steps as above
    ////but I'll start from the middle vertex and go to the bottom.
    //for (int y = Verts[1].y; y >= Verts[2].y; y--)
    //{
    //    //stepLong still calculates from top to bottom     Bottom  <-----  Top
    //    float stepLong = (float)(y - Verts[0].y) / (float)(Verts[2].y - Verts[0].y);

    //    //stepShort calculates from middle to bottom       Bottom  <-----  Middle
    //    float stepShort = (float)(y - Verts[1].y) / (float)(Verts[2].y - Verts[1].y);

    //    //fillStart position will increment from the middle X position towards the bottom x position.
    //    float fillStart = Verts[1].x + stepShort * (Verts[2].x - Verts[1].x);
    //    //fillEnd position will increment from the Top X position towards the bottom x position.
    //    float fillEnd = Verts[0].x + stepLong * (Verts[2].x - Verts[0].x);

    //    //Once again, we check if we are starting from the left and going to right, and swap if that's the case.
    //    if (fillStart > fillEnd) std::swap(fillStart, fillEnd);

    //    DrawLine((int)fillStart, y, (int)fillEnd, y, color);
    //}
}

void Renderer::WriteFramebufferToFile()
{
    mFramebuffer.write_tga_file("../build/framebuffer.tga");
}
