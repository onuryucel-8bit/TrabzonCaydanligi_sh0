//#include "ObjReader.h"
//
//ObjReader::ObjReader()
//{
//}
//
//ObjReader::~ObjReader()
//{
//}
//
//void ObjReader::read(std::string path)
//{
//    vertices.clear();
//    faces.clear();
//
//    std::ifstream file(path);    
//
//    std::string line;
//    while (std::getline(file, line))
//    {
//        std::stringstream ss(line);
//        std::string prefix;
//        ss >> prefix;
//
//        if (prefix == "v")
//        {
//            Vector3 vec;
//            ss >> vec.x >> vec.y >> vec.z;
//            vertices.push_back(vec);
//        }
//        else if (prefix == "f")
//        {
//            Face face;
//            std::string a, b, c, d;
//            ss >> a >> b >> c >> d;
//
//            int f1 = parseIndex(a);
//            int f2 = parseIndex(b);
//            int f3 = parseIndex(c);
//            int f4 = parseIndex(d);
//
//            f1--;
//            f2--;
//            f3--;
//            f4--;
//
//            faces.push_back({f1, f2, f3});
//            faces.push_back({f1, f3, f4});
//        }
//    }   
//}
//
//int ObjReader::parseIndex(std::string& str)
//{
//    std::stringstream ss(str);
//    int index;
//    ss >> index;
//    return index;
//}
