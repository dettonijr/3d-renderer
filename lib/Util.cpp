#include "Util.h"

float edgeFunction(const Point<float>& v0, const Point<float>& v1, const Point<float>& p) {
    return (p.x-v0.x)*(v1.y-v0.y) - (p.y-v0.y)*(v1.x-v0.x);
}

