#ifndef POINT_H_
#define POINT_H_

#include <cmath>
#include <cassert>
#include <vector>
#include <cstring>

class Transform;

template<typename T>
class Point final {
public:
    T x;
    T y;
    T z;
    
    Point() : x(0), y(0), z(0) {
    }

    Point(T x0, T y0, T z0) : x(x0), y(y0), z(z0) {
    }

    ~Point() { 
    }
    
    Point<T> operator-(const Point<T>& o) const {
        return Point<T>(x-o.x, y-o.y, z-o.z);
    }
    
    Point<T> operator+(const Point<T>& o) const {
        return Point<T>(x+o.x, y+o.y, z+o.z);
    }
    
    Point<T> operator*(T c) const {
        return Point<T>(x*c, y*c, z*c);
    }
    
    T operator*(const Point<T>& o) const {
        return x*o.x + y*o.y + z*o.z;
    }

    T norm() const{ 
        return std::sqrt(x*x + y*y + z*z); 
    }
    
    Point<T> normalize() const {
        return (*this)*(1.0/norm()); 
    }

    Point<T> operator*(const Transform& a) {
        return a*(*this); 
    }
};

class Transform {
    float m[4][4];   
 
public:
    Transform() {
        std::memset(m, 0, sizeof(m));
    }
    
    float& operator()(const int i, const int j) {
        return m[i][j];
    }

    static Transform identity() {
        Transform ret;
        ret.m[0][0] = 1.;
        ret.m[1][1] = 1.;
        ret.m[2][2] = 1.;
        ret.m[3][3] = 1.;
        return ret;
    }
    
    static Transform translate(float x, float y, float z) {
        Transform ret = identity();
        ret.m[0][3] = x;
        ret.m[1][3] = y;
        ret.m[2][3] = z;
        return ret;
    }

    static Transform scale(float sx, float sy, float sz) {
        Transform ret = identity();
        ret.m[0][0] = sx;
        ret.m[1][1] = sy;
        ret.m[2][2] = sz;
        return ret;
    }

    static Transform rotX(float alpha) {
        Transform ret = identity();
        float sin = std::sin(alpha);
        float cos = std::cos(alpha);
        ret.m[1][1] = cos;
        ret.m[1][2] = -sin;
        ret.m[2][1] = sin;
        ret.m[2][2] = cos;
        return ret;
    }
    
    static Transform rotY(float alpha) {
        Transform ret = identity();
        float sin = std::sin(alpha);
        float cos = std::cos(alpha);
        ret.m[0][0] = cos;
        ret.m[0][2] = sin;
        ret.m[2][0] = -sin;
        ret.m[2][2] = cos;
        return ret;
    }
    
    static Transform rotZ(float alpha) {
        Transform ret = identity();
        float sin = std::sin(alpha);
        float cos = std::cos(alpha);
        ret.m[0][0] = cos;
        ret.m[0][1] = -sin;
        ret.m[1][0] = sin;
        ret.m[1][1] = cos;
        return ret;
    }

    Transform operator*(const Transform& a) const {
        Transform ret;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                ret.m[i][j] = 0.0;
                for (int k = 0; k < 4; k++) {
                    ret.m[i][j] += m[i][k] * a.m[k][j];
                }
            }
        }
        return ret;
    }
    
    Point<float> operator*(const Point<float>& a) const {
        float x = m[0][0]*a.x + m[0][1]*a.y + m[0][2]*a.z + m[0][3];
        float y = m[1][0]*a.x + m[1][1]*a.y + m[1][2]*a.z + m[1][3];
        float z = m[2][0]*a.x + m[2][1]*a.y + m[2][2]*a.z + m[2][3];
        return Point<float>(x,y,z);
    }
    
};


#endif
