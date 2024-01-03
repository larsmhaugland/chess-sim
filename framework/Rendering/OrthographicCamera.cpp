
#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"


/*

void OrthographicCamera::RecalculateViewProjectionMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f),Position) *
            glm::rotate(glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));
    ViewMatrix = glm::inverse(transform);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}*/

void OrthographicCamera::RecalculateMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f),Position) *
            glm::rotate(glm::mat4(1.0f), Rotation, glm::vec3(0,0,1));

    ViewMatrix = glm::inverse(transform);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}




OrthographicCamera::OrthographicCamera(const Frustrum& frustrum ,const glm::vec3 &position, float rotation) {
    this->CameraFrustrum = frustrum;
    SetPosition(position);
    SetRotation(rotation);
    RecalculateMatrix();
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}
