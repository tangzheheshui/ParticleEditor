#include "camera.h"
#include <QtCore/QDebug>

const float YAW         = 0.0f;
const float PITCH       =  0.0f;
const unsigned int SCR_WIDTH = 614;
const unsigned int SCR_HEIGHT = 455;

Camera::Camera()
    : _yaw(YAW)
    , _pitch(PITCH)
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
    {
        QVector3D f = (_position).normalized();
        QVector3D s = QVector3D::crossProduct(f, _worldUp).normalized();
        QVector3D u = QVector3D::crossProduct(s, f);

        QMatrix4x4 result;
        result.setColumn(0, QVector4D(s, 0));
        result.setColumn(1, QVector4D(u, 0));
        result.setColumn(2, QVector4D(-f, 0));
        result.setColumn(3, QVector4D(0, 0, 0, 1));

        QMatrix4x4 temp;
        temp.translate(-_position);
        //lookat = result * temp;
    }
    QMatrix4x4 projection;
    projection.perspective((_fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, _near, _far);
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
        qDebug() << "_yaw = " << _yaw << ", _pitch = " << _pitch;
        return; // 第一次不需要计算pos 和 target
    }

    auto dir = _position - _target;
    float r = dir.length();
    float y = r * sin(qDegreesToRadians(_pitch));
    float x = r * cos(qDegreesToRadians(_pitch)) * sin(qDegreesToRadians(_yaw));
    float z = r * cos(qDegreesToRadians(_pitch)) * cos(qDegreesToRadians(_yaw));
    _position = {x, y, z};

    qDebug() << "cam_pos = " << _position;
}

Camera& Camera::GetCamera() {
    static Camera cam;
    return cam;
}

void Camera::screenToWorld(const QVector2D &screen, QVector3D &world) {
    QVector4D proj(screen.x() * 2 / SCR_WIDTH - 1, screen.y() * 2 / SCR_HEIGHT, -1, 1);
    auto vpMat = GetCamera().GetVPMatrix();
    QVector4D worldNear = vpMat.inverted() * proj;
    world = {worldNear.x() / worldNear.w(), worldNear.y() / worldNear.w(), worldNear.z() / worldNear.w()};
}
