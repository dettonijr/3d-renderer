#include "Obj.h"
#include <string>
#include <sstream>
#include <fstream>


Obj::Obj(const char * filename, TGAFile& texture_file) : texture(texture_file) {
    std::ifstream in(filename, std::ifstream::in);
    
    std::vector<Point<float>> verts;
    std::vector<Point<float>> texture_points;

    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream ss(line.c_str());
        std::string command;
        ss >> command;
        if (command == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            Point<float> p(x, y, z);
            verts.push_back(p);
        } else if (command == "vt") { 
            float x, y, z;
            ss >> x >> y >> z;
            Point<float> p(x, y, z);
            texture_points.push_back(p);
        } else if (command == "f") { 
            Vertex vertex;
            int idx;
            int idxtexture;
            char trash;
            int itrash;
            uint32_t currFaceComponent = 0;
            while(ss >> idx) {
                ss >> trash;
                ss >> idxtexture;
                ss >> trash;
                ss >> itrash;
                idx--;
                idxtexture--;
                vertex.v[currFaceComponent] = verts[idx];
                vertex.vt[currFaceComponent] = texture_points[idxtexture];
                ++currFaceComponent;
            }
            faces.push_back(vertex);
        }
    }
}

Obj::~Obj() 
{

}

void Obj::transform(const Transform& f) {
    for (auto face = faces.begin(); face != faces.end(); face++) {
        face->v[0] = f*face->v[0];
        face->v[1] = f*face->v[1];
        face->v[2] = f*face->v[2];
    }
}
   
