#ifndef QUAT_H
#define QUAT_H

#include <iostream>
#include <cmath>
#include "Mat4x4.h"
#include "Utils.h"

struct Quat {
    float w, x, y, z;

    inline Quat(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : w(w), x(x), y(y), z(z) {}

    inline Quat operator*(const Quat& other) const {
        return Quat(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w
        );
    }
};

static inline Quat fromAxisAngle(const float angle, const float axisX, const float axisY, const float axisZ) {
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sinf(halfAngle);
    return Quat(cosf(halfAngle), axisX * sinHalfAngle, axisY * sinHalfAngle, axisZ * sinHalfAngle);
}

inline Quat normalise(Quat q) {
    float magnitude = std::sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    return Quat(q.w / magnitude, q.x / magnitude, q.y / magnitude, q.z / magnitude);
}

inline Quat conjugate(Quat q) {
    return Quat(q.w, -q.x, -q.y, -q.z);
}

inline Mat4x4 quatToMatrix(Quat q) {
    Mat4x4 matrix;
    matrix[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    matrix[0][1] = 2.0f * (q.x * q.y - q.z * q.w);
    matrix[0][2] = 2.0f * (q.x * q.z + q.y * q.w);
    matrix[1][0] = 2.0f * (q.x * q.y + q.z * q.w);
    matrix[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
    matrix[1][2] = 2.0f * (q.y * q.z - q.x * q.w);
    matrix[2][0] = 2.0f * (q.x * q.z - q.y * q.w);
    matrix[2][1] = 2.0f * (q.y * q.z + q.x * q.w);
    matrix[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    matrix[3][3] = 1.0f;
    return matrix;
}

// Convert quaternion to Euler angles (pitch, yaw, roll)
inline void quatToEulerAngles(Quat q, float &pitch, float &yaw, float &roll) {
    // Pitch (x-axis)
    float sinPitch = 2.0f * (q.w * q.x + q.y * q.z);
    float cosPitch = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    pitch = std::atan2(sinPitch, cosPitch);

    // Yaw (y-axis)
    float sinYaw = 2.0f * (q.w * q.y - q.z * q.x);
    yaw = std::asin(clamp(sinYaw, -1.0f, 1.0f));  // Clamp to avoid NaN

    // Roll (z-axis)
    float sinRoll = 2.0f * (q.w * q.z + q.x * q.y);
    float cosRoll = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    roll = std::atan2(sinRoll, cosRoll);
}

#endif