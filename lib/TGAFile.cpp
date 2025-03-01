#include "TGAFile.hpp"
#include <fstream>
#include <cassert>
#include <stdio.h>

TGAFile::TGAFile(const char* file) {
    assert(sizeof(header) == 18);

    std::ifstream in(file, std::ifstream::in);

    in.read((char*)&header, 18);
    
    // dump header for each field
    data.reserve(header.width*header.height);

    while (data.size() <= header.width*header.height) {
        unsigned char len = 0;
        std::uint32_t color = 0;
        in.read((char*)&len, 1);
        unsigned char type = len >> 7;
        len &= 0x7f;
        if (type == 1) {
            in.read((char*)&color, 3);
            for (int i = 0; i <= len; i++) {
                data.push_back(color);
            } 
        } else {
            for (int i = 0; i <= len; i++) {
                in.read((char*)&color, 3);
                data.push_back(color);
            } 
        }

    }
} 

Color TGAFile::get_color(float x, float y) const {
    assert(x >= 0.0 && x <= 1.0);
    assert(y >= 0.0 && y <= 1.0);
    std::size_t sx = x*header.width;
    std::size_t sy = y*header.height;
    std::uint32_t c = data[sx+sy*header.width];

    return Color(c>>16&0xff, c>>8&0xff, c&0xff);
}
