#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "math/Vector3.h"
#include "Triangle.h"

class ObjReader
{
public:
	ObjReader();
	~ObjReader();

    void read(std::string path);

	std::vector<Vector3> vertices;
	std::vector<Face> faces;
	
private:
	int parseIndex(std::string& str);

};