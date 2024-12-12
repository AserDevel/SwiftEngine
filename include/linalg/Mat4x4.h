#ifndef MAT4X4_H
#define MAT4X4_H

#include "Vec3.h"
#include "Vec4.h"
#include <array>

struct Mat4x4 {
    float data[4][4] = { 0 };

    Mat4x4() {};
    Mat4x4(float i);

    float* operator[](std::size_t row) { return data[row]; }
    const float* operator[](std::size_t row) const { return data[row]; }
    Vec4 operator*(const Vec4& i);
    Vec3 operator*(const Vec3& i);
    Mat4x4 operator*(const Mat4x4& m);

    void print();
};

Mat4x4 MatrixRotationX(float fAngleRad);

Mat4x4 MatrixRotationY(float fAngleRad);

Mat4x4 MatrixRotationZ(float fAngleRad);

Mat4x4 MatrixRotation(float xRad, float yRad, float zRad);

Mat4x4 MatrixProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

Mat4x4 MatrixTranslation(float x, float y, float z);

Mat4x4 MatrixScaling(float sx, float sy, float sz);

Mat4x4 MatrixWorld(Vec3 pos, Vec3 dir, Vec3 scale);

Mat4x4 MatrixLookAt(Vec3& eye, Vec3& target, Vec3& up);

#endif