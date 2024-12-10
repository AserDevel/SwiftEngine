#include "graphics/Camera.h"
#include "linalg/Mat4x4.h"
#include <math.h>

Mat4x4 Camera::getMatView() {
    Vec3 vTarget = position + front;
    return MatrixLookAt(position, vTarget, up);
}

Mat4x4 Camera::getMatProj() {
    return MatrixProjection(fov, aspectRatio, nearPlane, farPlane);
}

Mat4x4 Camera::getMatCamera() {
    return getMatProj() * getMatView();
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
    if (direction == "UP")
        position.y += velocity;
    if (direction == "DOWN")
        position.y -= velocity;

}

void Camera::processMouseInput(float xOffset, float yOffset, float sensitivity) {
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw -= xOffset;
    pitch += yOffset;

    // Constrain pitch to avoid gimbal lock
    if (pitch > PI / 2.01f) pitch = PI / 2.01f;
    if (pitch < -PI / 2.01f) pitch = -PI / 2.01f;

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