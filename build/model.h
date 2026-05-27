#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include "../tgaimage.h"
#include "Renderer.h"

class Model {
private:
	std::vector<Vector3D> verticesPositions;
	std::vector<Face> faces;

public:
	Model(std::string path);
	void Draw(std::shared_ptr<Renderer>& renderer);
};