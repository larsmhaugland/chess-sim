#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"
#include "GLFWApplication.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <array>


class PerspectiveCamera : public Camera
{
public:

    struct Frustrum {
        float angle;
        float width;
        float height;
        float near;
        float far;
    };

public:
    PerspectiveCamera(const Frustrum& frustrum = { 45.0f, 1024.0f, 1024.0f, 0.1f, -1.0f },
        const glm::vec3& position = glm::vec3(0.0f, 1.4f, 1.0f),
        const glm::vec3& lookAt = glm::vec3(-1.0f),
        const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f)){

        Position = position;
        LookAt = lookAt;
        UpVector = upVector;
        CameraFrustrum = frustrum;
        Camera::ViewMatrix = glm::lookAt(position, lookAt, upVector);
        Camera::ProjectionMatrix = glm::perspective(CameraFrustrum.angle,
                                        CameraFrustrum.width / CameraFrustrum.height, CameraFrustrum.near, CameraFrustrum.far);
        Camera::ViewProjectionMatrix = Camera::ProjectionMatrix * Camera::ViewMatrix;
    }
    ~PerspectiveCamera() = default;
    PerspectiveCamera(const PerspectiveCamera& camera) : Camera(camera)
    {
        this->LookAt = camera.LookAt;
        this->UpVector = camera.UpVector;
        this->CameraFrustrum = camera.CameraFrustrum;
    }

    void SetFrustrum(const Frustrum& frustrum)
    {
        this->CameraFrustrum = frustrum;
        this->RecalculateMatrix();
    }

    void SetLookAt(const glm::vec3& lookAt)
    {
        this->LookAt = lookAt;
        this->RecalculateMatrix();
    }

    const glm::vec3& GetLookAt() const
    {
        return this->LookAt;
    }

    void SetUpVector(const glm::vec3& upVector)
    {
        this->UpVector = upVector;
        this->RecalculateMatrix();
    }

    const Frustrum GetFrustrum() const
    {
        return this->CameraFrustrum;
    }

    const glm::vec3 GetUpVector() const
    {
        return this->UpVector;
    }

    void RotateCamera(float angle)
    {
        // Calculate the direction vector from LookAt to Position
        glm::vec3 direction = Position - LookAt;

        // Construct a rotation matrix to rotate around the Y-axis (up vector)
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, UpVector);

        // Apply the rotation to the direction vector
        direction = glm::vec3(rotationMatrix * glm::vec4(direction, 1.0f));

        // Update the Position
        Position = LookAt + direction;
        RecalculateMatrix();
    }

    void ZoomCamera(float zoomSpeed)
    {
        // Calculate the camera's forward vector (opposite of the look direction)
        glm::vec3 forward = glm::normalize(LookAt - Position);

        if(zoomSpeed > 0) {
            // Ensure that the camera doesn't get too close
            if (glm::distance(Position, LookAt) > 0.5f)
                Position += forward * zoomSpeed;  // Move the camera closer to the LookAt point
        } else
        {
            // Ensure that the camera doesn't get too far
            if (glm::distance(Position, LookAt) < 3.0f)
                // Move the camera away from the LookAt point
                Position += forward * zoomSpeed;
        }
        // Recalculate the view and projection matrices
        RecalculateMatrix();
    }


protected:
    void RecalculateMatrix() {
        Camera::ViewMatrix = glm::lookAt(Camera::Position, LookAt, UpVector);
        Camera::ProjectionMatrix = glm::perspective(glm::radians(CameraFrustrum.angle), CameraFrustrum.width / CameraFrustrum.height, CameraFrustrum.near, CameraFrustrum.far);
        Camera::ViewProjectionMatrix =  Camera::ProjectionMatrix * Camera::ViewMatrix;
    }

protected:
    glm::vec3 LookAt;
    glm::vec3 UpVector;
    Frustrum CameraFrustrum;
};

#endif // PERSPECTIVECAMERA_H_
