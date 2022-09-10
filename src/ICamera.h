#pragma once

#include "Util.h"
#include "Logger.h"
#include "State.h"

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

inline std::string to_string(CameraMovement cameraMovement) {
    switch (cameraMovement) {
        case CameraMovement::FORWARD:
            return "forward";
        case CameraMovement::BACKWARD:
            return "backward";
        case CameraMovement::LEFT:
            return "left";
        case CameraMovement::RIGHT:
            return "right";
        default:
            return "unknown";
    }
}

struct CameraProperties {
    float fov{ 45.0f };
    float aspect{ 16.0f / 9.0f };
    float near{ 0.1f };
    float far{ 100.0f };

    float speed{ 2.5f };
    float sensitivity{ 0.1f };

    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 right{ 1.0f, 0.0f, 0.0f };
    glm::vec3 front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 position{ 0.0f, 0.0f, -3.0f };
};

class ICamera {
public:
    ICamera(CameraProperties cameraProperties) : _cameraProperties{ cameraProperties } {}

    ICamera(CameraProperties cameraProperties, State* state) : _cameraProperties{ cameraProperties }, _state{ state } {}

    virtual ~ICamera() = default;

    virtual void init() {}

    virtual void handleMouse(float xPos, float yPos) {
        if (_viewUpdated) {
            _viewUpdated = false;
        }

        if (_state && _state->gameState() != GameState::GAME) {
            return;
        }

        auto deltaX = xPos - _xPosLast;
        auto deltaY = _yPosLast - yPos;
        if (_xPosLast < 0) {
            deltaX = 0;
            deltaY = 0;
        }

        _xPosLast = xPos;
        _yPosLast = yPos;

        _yaw += deltaX * _cameraProperties.sensitivity;
        _pitch += deltaY * _cameraProperties.sensitivity;

        if (_pitch > 89.0f) {
            _pitch = 89.0f;
        }
        if (_pitch < -89.0f) {
            _pitch = -89.0f;
        }

        while (_yaw < -180.0f) {
            _yaw += 360.0f;
        }
        while (_yaw > 180.0f) {
            _yaw -= 360.0f;
        }

        //if (deltaX > 0.0f || deltaY > 0.0f) { // TODO: inspect error
            updateVectors();
            view();

            _viewUpdated = true;
        //}
    }

    virtual const bool& viewUpdated() {
        return _viewUpdated;
    }

    virtual const glm::mat4& getView() {
        return _view;
    }

    virtual const glm::mat4& getProjection() {
        return _projection;
    }

    glm::vec3 projectionVector(glm::vec3& front, glm::vec3& right) {
        glm::vec3 u = glm::cross(_cameraProperties.up, right);
        glm::vec3 v = front;

        return ((u * v) / static_cast<float>(pow(glm::length(u), 2)) * u);
    }

    virtual CameraProperties& getCameraProperties() {
        return _cameraProperties;
    }

protected:
    virtual void view() {
        _view = glm::lookAt(
            _cameraProperties.position,
            _cameraProperties.position + _cameraProperties.front,
            _cameraProperties.up
        );
    }

    virtual void projection() {
        _projection = glm::perspective(
            glm::radians(_cameraProperties.fov),
            _cameraProperties.aspect,
            _cameraProperties.near,
            _cameraProperties.far
        );
    }

    virtual void updateVectors() {
        glm::vec3 eulerAngle{};
        eulerAngle.x = cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
        eulerAngle.y = sin(glm::radians(_pitch));
        eulerAngle.z = cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));

        _cameraProperties.front = glm::normalize(eulerAngle);
        _cameraProperties.right = glm::normalize(glm::cross(_cameraProperties.front, _cameraProperties.up));
    }

    CameraProperties _cameraProperties;
    State* _state = nullptr;
    glm::mat4 _view{};
    glm::mat4 _projection{};

    float _roll = 0.0f;
    float _pitch = 0.0f;
    float _yaw = 90.0f;
    float _xPosLast = -1.0f;
    float _yPosLast = -1.0f;
    bool _viewUpdated = false;

};
