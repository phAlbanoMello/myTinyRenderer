#include <string>
#include "build/model.h"

const std::string objPath = "C:/Repos/myTinyRenderer/obj/diablo3_pose/diablo3_pose.obj";

int main(int argc, char** argv) {
    //std::shared_ptr<Model> model = std::make_shared<Model>(objPath);
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(Renderer());

    //model->Draw(renderer);

    renderer->DrawTriangle(7, 45, 35, 100, 45, 60, Colors::red);
    renderer->DrawTriangle(120, 35, 90, 5, 45, 110, Colors::white);
    renderer->DrawTriangle(115, 83, 80, 90, 85, 120, Colors::blue);
    renderer->WriteFramebufferToFile();

    return 0;
}