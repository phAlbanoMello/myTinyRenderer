#include <string>
#include "build/model.h"

const std::string objPath = "C:/Repos/myTinyRenderer/obj/diablo3_pose/diablo3_pose.obj";

int main(int argc, char** argv) {
    std::shared_ptr<Model> model = std::make_shared<Model>(objPath);
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(Renderer());

    model->Draw(renderer);

    return 0;
}