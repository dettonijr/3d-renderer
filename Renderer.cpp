#include "Renderer.h"
#include "Util.h"
#include <limits>

#define I2F(x) ((float)x+0.5)
#define F2I(x) ((int)x)

Renderer::Renderer(Framebuffer& fb) : framebuffer(fb) {
}

Renderer::~Renderer() {
}

void Renderer::add_obj(Obj& o) {
    objs.push_back(o);
}

void Renderer::set_light(Point<float> light_vec) {
    this->light_vec = light_vec;
}

void Renderer::render_all_objs(const Transform& t) {
    // Get references to framebuffer properties
    int width = framebuffer.width();
    int height = framebuffer.height();
    std::vector<float>& zbuf = framebuffer.get_zbuffer();
    
    // Clear the zbuffer at the beginning of rendering
    framebuffer.clear_zbuffer();

    for (auto obj = objs.begin(); obj != objs.end(); ++obj) {
        auto& faces = obj->get_faces();
        for (int i = 0; i < faces.size(); i++) {
            auto face = faces[i];
            Point<float>& v0 = face.v[0];
            Point<float>& v1 = face.v[1];
            Point<float>& v2 = face.v[2];
            Point<float>& t0 = face.vt[0];
            Point<float>& t1 = face.vt[1];
            Point<float>& t2 = face.vt[2];
            
            Point<float> p0 = v0*t;
            Point<float> p1 = v1*t;
            Point<float> p2 = v2*t;

            Point<float> a = p2-p0; 
            Point<float> b = p1-p0;

            float nx = a.y*b.z - a.z*b.y; 
            float ny = a.z*b.x - a.x*b.z; 
            float nz = a.x*b.y - a.y*b.x; 

            const Point<float> n(nx,ny,nz);
            const Point<float> normalized = n.normalize();
            float intensity = normalized*light_vec;

            if (intensity > 0) {
                TextureShader s{p0, p1, p2, t0, t1, t2, intensity, obj->texture};
                // FlatShader s{p0, p1, p2, Color::White*intensity};
                draw_triangle(p0, p1, p2, s); 
            }
        }
    }
}

void Renderer::draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, const Color& c) {
    int width = framebuffer.width();
    int height = framebuffer.height();
    std::vector<float>& zbuf = framebuffer.get_zbuffer();

    int minx = width-1;
    int maxx = 0;
    int miny = height-1;
    int maxy = 0;

    minx = std::min(F2I(v0.x), minx);
    minx = std::min(F2I(v1.x), minx);
    minx = std::min(F2I(v2.x), minx);
    minx = std::max(0, minx);
    miny = std::min(F2I(v0.y), miny);
    miny = std::min(F2I(v1.y), miny);
    miny = std::min(F2I(v2.y), miny);
    miny = std::max(0, miny);
    maxx = std::max(F2I(v0.x), maxx);
    maxx = std::max(F2I(v1.x), maxx);
    maxx = std::max(F2I(v2.x), maxx);
    maxx = std::min(width-1, maxx);
    maxy = std::max(F2I(v0.y), maxy);
    maxy = std::max(F2I(v1.y), maxy);
    maxy = std::max(F2I(v2.y), maxy);
    maxy = std::min(height-1, maxy);

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            float w0 = edgeFunction(v1,v2,Point<float>(I2F(x),I2F(y),0));
            float w1 = edgeFunction(v2,v0,Point<float>(I2F(x),I2F(y),0));
            float w2 = edgeFunction(v0,v1,Point<float>(I2F(x),I2F(y),0));
            bool inside = w0>=0 && w1>=0 && w2>=0;
            if (inside) {
                float z = v0.z * w0 + v1.z*w1 + v2.z*w2;
                if (z < zbuf[y*width+x]) {
                    zbuf[y*width+x] = z;
                    framebuffer.set_pixel(x,y,c);
                }
            }
        }
    }
}

void Renderer::draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, FragmentShader& shader) {
    int width = framebuffer.width();
    int height = framebuffer.height();
    std::vector<float>& zbuf = framebuffer.get_zbuffer();

    int minx = width-1;
    int maxx = 0;
    int miny = height-1;
    int maxy = 0;

    minx = std::min(F2I(v0.x), minx);
    minx = std::min(F2I(v1.x), minx);
    minx = std::min(F2I(v2.x), minx);
    minx = std::max(0, minx);
    miny = std::min(F2I(v0.y), miny);
    miny = std::min(F2I(v1.y), miny);
    miny = std::min(F2I(v2.y), miny);
    miny = std::max(0, miny);
    maxx = std::max(F2I(v0.x), maxx);
    maxx = std::max(F2I(v1.x), maxx);
    maxx = std::max(F2I(v2.x), maxx);
    maxx = std::min(width-1, maxx);
    maxy = std::max(F2I(v0.y), maxy);
    maxy = std::max(F2I(v1.y), maxy);
    maxy = std::max(F2I(v2.y), maxy);
    maxy = std::min(height-1, maxy);

    for (int y = miny; y <= maxy; y++) {
        for (int x = minx; x <= maxx; x++) {
            float z;
            Color c;
            bool discard = shader.fragment(x, y, z, c);
            if (!discard && z < zbuf[y*width+x]) {
                zbuf[y*width+x] = z;
                framebuffer.set_pixel(x,y,c);
            }
        }
    }
}

void Renderer::draw_triangle(const Point<int>& v0, const Point<int>& v1, const Point<int>& v2, const Color& c) {
    // Implementation for integer points - probably not used much but keeping for compatibility
    draw_triangle(Point<float>(v0.x, v0.y, v0.z), Point<float>(v1.x, v1.y, v1.z), Point<float>(v2.x, v2.y, v2.z), c);
}

void Renderer::draw_line(const Point<float>& p0, const Point<float>& p1, const Color& c) {
    draw_line(Point<int>(p0.x,p0.y,p0.z), Point<int>(p1.x,p1.y,p1.z), c);
}

void Renderer::draw_line(const Point<int>& p0, const Point<int>& p1, const Color& c) {
    draw_line(p0.x, p0.y, p1.x, p1.y, c);
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, const Color& c) {
    int width = framebuffer.width();
    int height = framebuffer.height();
    
    // TODO clipping
    int dx = std::abs(x0 - x1); 
    int dy = std::abs(y0 - y1);
    if (dx > dy) {
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        float deltaerr = dy/(float)dx;
        float err = deltaerr - 0.5f;

        int y = y0;
        for (int x = x0; x <= x1; x++) {
            err += deltaerr;
            if (err >= 0.5f) {
                err -= 1.0f;
                y += (y0 > y1 ? -1 : 1);
            }
            framebuffer.set_pixel(x, y, c); 
        } 
    } else {
        if (y0 > y1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        float deltaerr = dx/(float)dy;
        float err = deltaerr - 0.5f;

        int x = x0;
        for (int y = y0; y <= y1; y++) {
            err += deltaerr;
            if (err >= 0.5f) {
                err -= 1.0f;
                x += (x0 > x1 ? -1 : 1);
            }
            framebuffer.set_pixel(x, y, c); 
        } 
    }
}

void Renderer::draw_rect(int minx, int miny, int maxx, int maxy, Color& c) {
    int width = framebuffer.width();
    int height = framebuffer.height();
    std::vector<uint32_t>& buf = framebuffer.get_raw_buffer();
    
    if (minx > width || miny > height)
        return;
    if (minx < 0)
        minx = 0;
    if (maxx > width)
        maxx = width;
    if (miny < 0)
        miny = 0;
    if (maxy > height)
        maxy = height;

    uint32_t color = c.r << 16 | c.g << 8 | c.b;

    for (int y = miny; y < maxy; y++) {
        uint32_t* pixelstart = &buf[y*width] + minx;
        uint32_t* pixelend = pixelstart + maxx - minx;
        while (pixelstart < pixelend) {
             *pixelstart++ = color;
        }
    }
}
