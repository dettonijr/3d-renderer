#ifndef _COLOR_H_
#define _COLOR_H_

#include <cstdint>
#include <random>
#include <cassert>

struct Color final{
    const uint8_t r;
    const uint8_t g;
    const uint8_t b;

    Color(uint8_t r0, uint8_t g0, uint8_t b0) : r(r0), g(g0), b(b0) {
    }

    ~Color() { 
    }

    Color operator*(float c) {
        assert(c >= 0.0 && c <= 1.0);
        return Color(r*c, g*c, b*c);
    }
    
    static Color random() {
        return Color(std::rand(), std::rand(), std::rand()); 
    }

    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
};

#endif
