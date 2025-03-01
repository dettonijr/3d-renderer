#ifndef _OBJ_H_
#define _OBJ_H_
#include "Point.h"
#include <vector>
#include <array>
#include "X11Framebuffer.hpp"
#include "TGAFile.hpp"

class Obj final{

    struct Vertex {
        Vertex() {
        }

        std::array<Point<float>, 3> v;
        std::array<Point<float>, 3> vt;
    };

public:
    Obj(const char * filename, TGAFile& texture);
    ~Obj();

    void transform(const Transform& f);

    const std::vector<Vertex>& get_faces() const {
        return faces;
    }

    TGAFile& texture;

private:
    std::vector<Vertex> faces;
};

#endif
