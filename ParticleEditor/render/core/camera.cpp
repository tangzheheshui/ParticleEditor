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
    _position = {0, 0, 0};
}

void Camera::setPosition(QVector3D pos) {
    _position = pos;
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
    projection.perspective(qRadiansToDegrees(_fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, _near, _far);
    return projection * lookat;
}

void Camera::caculate() {
    if (!_needCal) {
        return;
    }
    _needCal = false;
    QVector3D front;
    front.setX(cos(qRadiansToDegrees(_yaw)) * cos(qRadiansToDegrees(_pitch)) );
    front.setY(sin(qRadiansToDegrees(_pitch)));
    front.setZ(sin(qRadiansToDegrees(_yaw)) * cos(qRadiansToDegrees(_pitch)));
    _front = front.normalized();

    // also re-calculate the Right and Up vector
    _right = QVector3D::crossProduct(_front, _worldUp).normalized();
    _up    = QVector3D::crossProduct(_right, _front).normalized();

    // 重新计算位置
    float r = _position.length();
    float y = r * sin(qRadiansToDegrees(_pitch));
    float x = r * cos(qRadiansToDegrees(_pitch)) * sin(qRadiansToDegrees(_yaw));
    float z = r * cos(qRadiansToDegrees(_pitch)) * cos(qRadiansToDegrees(_yaw));
    _position = {x, y, z};

    //qDebug() << "cam_pos = " << _position;
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
