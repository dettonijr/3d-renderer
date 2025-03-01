#include "Obj.h"
#include "X11Framebuffer.hpp"
#include "Shader.h"
#include <vector>

class World;  // Forward declaration

class Renderer {
public:
    Renderer(X11Framebuffer& fb);
    ~Renderer();

    void set_light(Point<float> light_vec);
    void render_world(const World& world, const Transform& t);

    void draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, const Color& c);
    void draw_triangle(const Point<float>& v0, const Point<float>& v1, const Point<float>& v2, FragmentShader& shader);
    void draw_line(int x0, int y0, int x1, int y1, const Color& c);
    void draw_rect(int minx, int miny, int maxx, int maxy, Color& c);

private:
    X11Framebuffer& framebuffer;
    Point<float> light_vec;
};
