#ifndef UTILS_H
#define UTILS_H

#define PI  3.14159f

inline float clamp(float target, float min, float max) {
    if (target < min) target = min;
    if (target > max) target = max;
    return target;
}

inline float toRad(const float degrees) {
    return degrees / (180 / PI); 
}

inline float toDegrees(const float radians) {
    return radians * (180 / PI);
}

#endif