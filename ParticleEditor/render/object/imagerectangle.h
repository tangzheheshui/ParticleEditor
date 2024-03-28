#ifndef IMAGERECTANGLE_H
#define IMAGERECTANGLE_H

#include <vector>
#include "BaseDraw.h"

class ImageRectangle : public BaseDraw {
public:
    ImageRectangle() {}
    ~ImageRectangle() {}
    virtual bool draw() override;
    void setPoints(const QVector3D &pos1, const QVector3D &pos2, const QVector3D &pos3, const QVector3D &pos4);
    void setImagePath(const QString &path, const QString &diffuseMap, const QString &normalMap, const QString &heightMap);
    void setSetp(float w, float h);
    void calculate();
private:
    void getTangent(const QVector2D &uv1, const QVector2D &uv2, const QVector2D &uv3, const QVector2D &uv4, QVector3D &tangent1, QVector3D &bitangent1, QVector3D &tangent2, QVector3D &bitangent2);
private:
    // 逆时针
    QVector3D _pos1;
    QVector3D _pos2;
    QVector3D _pos3;
    QVector3D _pos4;
    QString m_image_diffuse;
    QString m_image_normal;
    QString m_image_height;
    QVector2D m_step;

    // 计算出来的顶点和索引
    struct Buffer {
        QVector3D pos;
        QVector2D coord;
        QVector3D normal;
        QVector3D tangent;
        QVector3D bitangent;
    };
    std::vector<Buffer> m_buffer;
};

#endif // IMAGERECTANGLE_H
