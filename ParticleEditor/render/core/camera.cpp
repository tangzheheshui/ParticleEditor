#include "camera.h"
#include <QtCore/QDebug>

Camera::Camera()
    : _yaw(0.f)
    , _pitch(0.f)
    , _fov(45)
{
    _near = 0.1f;
    _far = 100.f;
    _position = _target = {0, 0, 0};
}

void Camera::setPosition(QVector3D pos) {
    _position = pos;
    _needCal = true;
}

void Camera::setTartet(QVector3D pos) {
    _target = pos;
    _needCal = true;
}

void Camera::setYaw(float yaw) {
    _yaw = yaw;
    _needCal = true;
}

void Camera::setPitch(float pitch) {
    _pitch = pitch;
    _needCal = true;
}

void Camera::setFov(float fov) {
    _fov = fov;
    _needCal = true;
}

QMatrix4x4 Camera::GetVPMatrix() {
    caculate();

    QMatrix4x4 lookat;
    lookat.lookAt(_position, QVector3D(), _worldUp);
    QMatrix4x4 projection;
    projection.perspective((_fov), _viewPort.x() / _viewPort.y(), _near, _far);
    return projection * lookat;
}

void Camera::caculate() {
    if (!_needCal) {
        return;
    }
    _needCal = false;

    // 重新计算位置
    if (_first) {
        auto dir = _position - _target;
        _yaw = atan2(dir.x(), dir.z());
        _yaw = qRadiansToDegrees(_yaw);
        _pitch = atan2(dir.y(), std::sqrtf(dir.x()*dir.x() + dir.z() * dir.x()));
        _pitch = qRadiansToDegrees(_pitch);
        _first = false;
        return; // 第一次不需要计算pos 和 target
    }

    auto dir = _position - _target;
    float r = dir.length();
    float y = r * sin(qDegreesToRadians(_pitch));
    float x = r * cos(qDegreesToRadians(_pitch)) * sin(qDegreesToRadians(_yaw));
    float z = r * cos(qDegreesToRadians(_pitch)) * cos(qDegreesToRadians(_yaw));
    _position = {x, y, z};
}

Camera& Camera::GetCamera() {
    static Camera cam;
    return cam;
}

void Camera::screenToWorld(const QVector2D &screen, QVector3D &world) {
    QVector4D proj(screen.x() * 2 / _viewPort.x() - 1, screen.y() * 2 / _viewPort.y(), -1, 1);
    auto vpMat = GetCamera().GetVPMatrix();
    QVector4D worldNear = vpMat.inverted() * proj;
    world = {worldNear.x() / worldNear.w(), worldNear.y() / worldNear.w(), worldNear.z() / worldNear.w()};
}
