#include "model.h"

Model::Model(std::string path)
{
    std::fstream file(path);//Load wavefront obj file

    if (!file.is_open())
    {
        std::cout << "Error opening wavefront file" << std::endl;
    }

    std::string parsedModel;

    while (std::getline(file, parsedModel))
    {
        if (parsedModel.substr(0, 2) == "v ")
        {
            std::istringstream iss(parsedModel);

            float x = 0.0;
            float y = 0.0;
            float z = 0.0;
            std::string skip;
            iss >> skip >> x >> y >> z;

            verticesPositions.push_back(Vector3D(x, y, z));
        }

        if (parsedModel.substr(0, 2) == "f ")
        {
            std::istringstream iss(parsedModel);
            std::string token;
            std::string skip;

            int a;
            int b;
            int c;

            iss >> skip;

            std::getline(iss, token, '/');
            a = std::stoi(token);
            iss >> skip;

            std::getline(iss, token, '/');
            b = std::stoi(token);
            iss >> skip;

            std::getline(iss, token, '/');
            c = std::stoi(token);

            faces.push_back(Face(a, b, c));
        }
    }

}

void Model::Draw(std::shared_ptr<Renderer>& renderer)
{
    for (size_t i = 0; i < faces.size(); i++)
    {
        Vector3D aVertexPos = renderer->ViewportTransform(verticesPositions[faces[i].a - 1]);
        Vector3D bVertexPos = renderer->ViewportTransform(verticesPositions[faces[i].b - 1]);
        Vector3D cVertexPos = renderer->ViewportTransform(verticesPositions[faces[i].c - 1]);

        renderer->DrawLine(aVertexPos.x, aVertexPos.y, bVertexPos.x, bVertexPos.y, Colors::red);
        renderer->DrawLine(bVertexPos.x, bVertexPos.y, cVertexPos.x, cVertexPos.y, Colors::red);
        renderer->DrawLine(cVertexPos.x, cVertexPos.y, aVertexPos.x, aVertexPos.y, Colors::red);
    }

    renderer->WriteFramebufferToFile();
}
