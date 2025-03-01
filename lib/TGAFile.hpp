#ifndef _TGAFILE_H_
#define _TGAFILE_H_

#include <cstdint>
#include <vector>
#include "Color.h"

struct TGAFile {
#pragma pack(push,1)
    struct {
        std::uint8_t id_length;
        std::uint8_t color_map_type;
        std::uint8_t image_type; 
        std::uint16_t colormap_start;
        std::uint16_t colormap_length;
        std::uint8_t color_depth;
        std::uint16_t x_origin;  
        std::uint16_t y_origin;  
        std::uint16_t width;    
        std::uint16_t height;   
        std::uint8_t depth;
        std::uint8_t image_descriptor; 
    } header;
#pragma pack(pop)

    std::vector<std::uint32_t> data;

    TGAFile(const char* file);
    Color get_color(float x, float y) const;
};

#endif
