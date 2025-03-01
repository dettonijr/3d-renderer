#include "Color.h"
#include "Point.h"
#include "TGAFile.hpp"

#define I2F(x) ((float)x+0.5)
#define F2I(x) ((int)x)

float edgeFunction(const Point<float>& v0, const Point<float>& v1, const Point<float>& p);

struct FragmentShader {
    virtual ~FragmentShader() {}
    virtual bool fragment(int x, int y, float& outZ, Color& outColor) const = 0;
};

struct TextureShader final : FragmentShader {
    const Point<float>& v0;        
    const Point<float>& v1;        
    const Point<float>& v2;        
    const Point<float>& t0;        
    const Point<float>& t1;        
    const Point<float>& t2;        
    const float intensity;
    const TGAFile& texture;     

    TextureShader(
        Point<float>& _v0,        
        Point<float>& _v1,        
        Point<float>& _v2,        
        Point<float>& _t0,        
        Point<float>& _t1,        
        Point<float>& _t2,        
        float _intensity,
        TGAFile& _texture) :
        v0(_v0), 
        v1(_v1),        
        v2(_v2),       
        t0(_t0),        
        t1(_t1),        
        t2(_t2),
        intensity(_intensity),
        texture(_texture) 
    {
    }

    ~TextureShader() {

    }

    bool fragment(int x, int y, float& outZ, Color& outColor) const override {
        float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
        float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
        float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
        if (w0 < 0 | w1 < 0 | w2 < 0)
            return true;
        float area = edgeFunction(v0,v1,v2);
        w0 /= area;
        w1 /= area;
        w2 /= area;
         
        float tx = t0.x * w0 + t1.x*w1 + t2.x*w2;
        float ty = t0.y * w0 + t1.y*w1 + t2.y*w2;
        
        outZ = v0.z * w0 + v1.z*w1 + v2.z*w2;
        outColor = texture.get_color(tx,ty)*intensity;
        return false;
    }
};

struct FlatShader final: FragmentShader {
    const Color c;     
    const Point<float>& v0;        
    const Point<float>& v1;        
    const Point<float>& v2;        

    FlatShader(
        const Point<float>& v0,        
        const Point<float>& v1,        
        const Point<float>& v2,
        const Color& c
    ) :
        v0(v0), 
        v1(v1),        
        v2(v2),       
        c(c)
    {
    }

    ~FlatShader() {

    }

    bool fragment(int x, int y, float& outZ, Color& outColor) const override {
        float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
        float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
        float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
        if (w0 < 0 | w1 < 0 | w2 < 0)
            return true;
        float area = edgeFunction(v0,v1,v2);
        w0 /= area;
        w1 /= area;
        w2 /= area;
        
        outZ = v0.z * w0 + v1.z*w1 + v2.z*w2;
        outColor = c;
        return false;
    }
};

struct BorderShader final: FragmentShader {
    const Color c;     
    const Point<float>& v0;        
    const Point<float>& v1;        
    const Point<float>& v2;        

    BorderShader(
        const Point<float>& v0,        
        const Point<float>& v1,        
        const Point<float>& v2,
        const Color& c
    ) :
        v0(v0), 
        v1(v1),        
        v2(v2),       
        c(c)
    {
    }

    ~BorderShader() {

    }

    bool fragment(int x, int y, float& outZ, Color& outColor) const override {
        float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
        float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
        float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
        if (w0 < 0 | w1 < 0 | w2 < 0)
            return true;
        float area = edgeFunction(v0,v1,v2);
        w0 /= area;
        w1 /= area;
        w2 /= area;
        
        if (w0 > 0.04 && w1 > 0.04 && w2 > 0.04)
            outColor = Color::Black;
        else
            outColor = c;       
 
        outZ = v0.z * w0 + v1.z*w1 + v2.z*w2;
        return false;
    }
};
