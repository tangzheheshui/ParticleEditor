#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light {
public:
    static Light& GlobalLight() {
        static Light light;
        light.position = {0.f, 10.0f, .0f};
        light.direction = -light.position;
        light.cosTheta = cos(qDegreesToRadians(15));
        float ambient = 0.4;
        light.ambient = QVector3D(ambient, ambient, ambient);
        float diffuse = 0.7;
        light.diffuse = QVector3D(diffuse, diffuse, diffuse);
        float specular = 0;
        light.specular = QVector3D(specular, specular, specular);
        return light;
    }
public:
    QVector3D position;
    QVector3D direction;
    float cosTheta = 0.f;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
};

#endif // LIGHT_H
