//#pragma once
//
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <vector>
//
//#include "math/Vector3.h"
//
//struct Triangle
//{
//	Vector2 points[3];
//	float averageDepth = 0;
//	Color_t color;
//};
//
//struct Face
//{
//	int a, b, c;
//};
//
//class ObjReader
//{
//public:
//	ObjReader();
//	~ObjReader();
//
//    void read(std::string path);
//	int parseIndex(std::string& str);
//
//	std::vector<Vector3> vertices;
//	std::vector<Face> faces;
//private:
//
//};