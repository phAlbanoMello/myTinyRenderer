#include <string>
#include "build/model.h"

const std::string objPath = "C:/Repos/myTinyRenderer/obj/diablo3_pose/diablo3_pose.obj";

int main(int argc, char** argv) {
    constexpr int width  = 1920;
    constexpr int height = 1920;

    Model model(objPath);
    model.DrawModelToFile(width, height);

    return 0;
}