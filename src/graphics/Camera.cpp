#include "graphics/Camera.h"
#include "linalg/Mat4x4.h"
#include <math.h>

Mat4x4 Camera::getViewMatrix() {
    Vec3 vTarget = position + front;
    return MatrixLookAt(position, vTarget, up);
}

Mat4x4 Camera::getProjectionMatrix() {
    return MatrixProjection(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::processKeyboardInput(const std::string& direction, float deltaTime, float speed) {
    float velocity = speed * deltaTime;
    if (direction == "FORWARD")
        position -= (front * velocity);
    if (direction == "BACKWARD")
        position += (front * velocity);
    if (direction == "LEFT")
        position -= (right * velocity);
    if (direction == "RIGHT")
        position += (right * velocity);
}

void Camera::processMouseInput(float xOffset, float yOffset, float sensitivity) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Constrain pitch to avoid gimbal lock
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update camera vectors
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Calculate the new front vector
    Vec3 newFront;
    newFront.x = cosf(yaw) * cosf(pitch);
    newFront.y = sinf(pitch);
    newFront.z = sinf(yaw) * cosf(pitch);
    front = normalise(newFront);

    // Recalculate right and up vectors
    right = cross(front, worldUp);
    right = normalise(right);

    up = cross(right, front);
    up = normalise(up);
}
