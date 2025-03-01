#include "Obj.h"
#include "Framebuffer.hpp"
#include "Shader.h"
#include <vector>

class World;  // Forward declaration

class Renderer {
public:
    Renderer(Framebuffer& fb);
    ~Renderer();

    void set_light(Point<float> light_vec);
    void render_world(const World& world, const Transform& t);

    // Drawing methods 
    void draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, const Color& c);
    void draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, FragmentShader& shader);
    void draw_triangle(const Point<int>& v0, const Point<int>& v1, const Point<int>& v2, const Color& c);

    void draw_line(const Point<float>& p0, const Point<float>& p1, const Color& c);
    void draw_line(const Point<int>& p0, const Point<int>& p1, const Color& c);
    void draw_line(int x0, int y0, int x1, int y1, const Color& c);
    void draw_rect(int minx, int miny, int maxx, int maxy, Color& c);

private:
    Framebuffer& framebuffer;
    Point<float> light_vec;
};
