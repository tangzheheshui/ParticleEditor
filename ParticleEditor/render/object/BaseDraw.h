#ifndef BASEDRAW_H
#define BASEDRAW_H

#include <vector>
#include <QVector3D>
#include <QOpenGLBuffer>

class BaseDraw {
public:
    BaseDraw() {}
    ~BaseDraw() {
        if (_VAO > 0) {
            glDeleteVertexArrays(1, &_VAO);
        }

        if (_VBO > 0) {
            glDeleteBuffers(1, &_VBO);
        }

        if (_EBO > 0) {
            glDeleteBuffers(1, &_EBO);
        }
    }
    virtual bool draw() = 0;
    virtual void update() {}

public:
    virtual void getDebugPoint(std::vector<QVector3D> &vertices, std::vector<unsigned int> &indices) {}

    bool getLightOpen() const { return _lightOpen; }
    void setLightOpen(const bool &b) { _lightOpen = b; }

    void setAlpha(float alpha) { _alpha = alpha; }
    float getAlpha() { return _alpha; }
protected:
    unsigned int _VAO = 0;
    unsigned int _VBO = 0;
    unsigned int _EBO = 0;
    bool _has_bind_vbo = false;
private:
    bool _lightOpen = true;
    float _alpha = 1.0f;
};

#endif // BASEDRAW_H
