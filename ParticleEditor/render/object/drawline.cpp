#include "drawline.h"
#include "../core/shadercache.h"
#include "../core/camera.h"

bool DrawLine::draw() {
    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Color);
    if (!shader) {
        return false;
    }

    shader->bind();

    calculate();

    // 矩阵
    QMatrix4x4 mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setUniformValue("uMatrixMVP", mpMatrix);

    // 颜色
    shader->setUniformValue("uColor", m_color.x(), m_color.y(), m_color.z());

    glLineWidth(m_fWidth);
    glDrawElements(GL_LINES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);

    shader->release();
    return true;
}

void DrawLine::calculate() {
    if (_VBO == 0) {
        glGenBuffers(1, &_EBO);
        glGenBuffers(1, &_VBO);
        glGenVertexArrays(1, &_VAO);
    }

    glBindVertexArray(_VAO);

    // bind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * m_points.size(), m_points.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
