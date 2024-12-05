#ifndef VEC4_H
#define VEC4_H

// Vec4 is treated the same as Vec4, but it has a fourth element 
// for a homogeneous value.
struct Vec4 {
    union { float r = 0, x; };
    union { float g = 0, y; };
    union { float b = 0, z; };
    union { float a = 1, w; };
    
    Vec4(float a = 0, float b = 0, float c = 0, float d = 1) : 
        x(a), y(b), z(c), w(d) {};
    
    float length();
    
    Vec4 operator+(const Vec4& v);
    void operator+=(const Vec4& v);
    Vec4 operator-(const Vec4& v);
    void operator-=(const Vec4& v);
    Vec4 operator-();
    Vec4 operator*(const float k);
    Vec4 operator/(const float k);
};

float dot(Vec4& v1, Vec4& v2);

Vec4 cross(Vec4& v1, Vec4& v2);

Vec4 normalise(Vec4& v);

#endif