#ifndef camera_hpp
#define camera_hpp

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
public:
    Camera();
    void setPosition(QVector3D pos);
    void setTartet(QVector3D pos);
    void setViewport(const QVector2D &port) { _viewPort = port; }
    void setYaw(float yaw);
    void setPitch(float pitch);
    void setFov(float fov);
    float getYaw() { return _yaw; }
    float getPitch() { return _pitch; }
    float getFov() { return _fov; }
    QVector3D getPossition() { return _position; }

    void caculate();
    QMatrix4x4 GetVPMatrix();
    static Camera& GetCamera();
    void screenToWorld(const QVector2D &screen, QVector3D &world);
private:
    QVector3D _position;
    QVector3D _target;
    const QVector3D _worldUp = {0, 1, 0};
    // euler Angles
    float _yaw;
    float _pitch;
    float _fov;
    float _near;
    float _far;
    bool _needCal = true;
    bool _first = true;
    QVector2D _viewPort;
};

#endif /* camera_hpp */
