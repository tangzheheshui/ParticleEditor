#ifndef DRAWLINE_H
#define DRAWLINE_H

#include "BaseDraw.h"

class DrawLine : public BaseDraw {
public:
    DrawLine() {}
    void setData(const std::vector<QVector3D> &points, const std::vector<unsigned int> &indices) {
        m_points = points;
        m_indices = indices;
    }
    void setColor(const QVector3D &color) { m_color = color;}
    void setWidth(float width) { m_fWidth = width; }
    virtual bool draw() override;
private:
    void calculate();
private:
    std::vector<QVector3D> m_points;
    std::vector<unsigned int> m_indices;
    QVector3D m_color;
    float m_fWidth = 1;
};

#endif // DRAWLINE_H
