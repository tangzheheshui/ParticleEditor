#include "imagerectangle.h"
#include "render/core/texturemng.h"
#include "render/core/shadercache.h"
#include "render/core/camera.h"
#include "render/core/scene.h"
#include <QOpenGLTexture>

bool ImageRectangle::draw() {
    if (_VBO == 0) {
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);
    }

    auto texture_normal = TextureMng::getInstance().getTexture(m_image_normal);
    bool has_normal = (texture_normal != nullptr);

    auto texture_height = TextureMng::getInstance().getTexture(m_image_height);
    bool has_height = (texture_height != nullptr);

    if (!_has_bind_vbo) {
        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Buffer) * m_buffer.size(), m_buffer.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // tangent
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
        // bitangent
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Buffer), (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);

        glBindVertexArray(0);
        _has_bind_vbo = true;
    }


    auto shader = ShaderCache::GetInstance().GetShader(ShaderType::Ground);
    if (!shader) {
        return false;
    }

    shader->bind();

    auto texture = TextureMng::getInstance().getTexture(m_image_diffuse);
    assert(texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->textureId());
    shader->setUniformValue("uTexture", 0);

    int indexTex = 1;
    glActiveTexture(GL_TEXTURE1);
    auto depTex = TextureMng::getInstance().getTexture(STR_DEPTH_TEXTURE);
    assert(depTex);
    glBindTexture(GL_TEXTURE_2D, depTex->textureId());
    shader->setUniformValue("uTextureShadowMap", indexTex++);

    if (has_normal) {
        glActiveTexture(GL_TEXTURE0 + indexTex);
        glBindTexture(GL_TEXTURE_2D, texture_normal->textureId());
        shader->setUniformValue("uTextureNormal", indexTex++);
        shader->setUniformValue("uHasNormal", 1);
    } else {
        shader->setUniformValue("uHasNormal", 0);
    }

    if (has_height) {
        glActiveTexture(GL_TEXTURE0 + indexTex);
        glBindTexture(GL_TEXTURE_2D, texture_height->textureId());
        shader->setUniformValue("uTextureHeight", indexTex++);
        shader->setUniformValue("uHasHeight", 1);
    } else {
        shader->setUniformValue("uHasHeight", 0);
    }

    // 矩阵
    QMatrix4x4 model;
    shader->setUniformValue("uModel", model);
    auto mpMatrix = Camera::GetCamera().GetVPMatrix();
    shader->setUniformValue("uVP", mpMatrix);
    shader->setUniformValue("uLightSpaceMatrix", Scene::getScene().GetLightVPMatrix());

    // 灯光
    Scene& scene = Scene::getScene();
    scene.setLightUniform(shader.get());

    // 相机位置
    auto cam_pos = Camera::GetCamera().getPossition();
    shader->setUniformValue("uCameraPos", cam_pos);

    // 透明度
    shader->setUniformValue("uAlpha", getAlpha());

    // 开启混合
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, (int)m_buffer.size());
    return true;
}

void ImageRectangle::setPoints(const QVector3D &pos1, const QVector3D &pos2, const QVector3D &pos3, const QVector3D &pos4) {
    _pos1 = pos1;
    _pos2 = pos2;
    _pos3 = pos3;
    _pos4 = pos4;
}

void ImageRectangle::setSetp(float w, float h) {
    m_step = {w, h};
}

void ImageRectangle::getTangent(const QVector2D &uv1, const QVector2D &uv2, const QVector2D &uv3, const QVector2D &uv4, QVector3D &tangent1, QVector3D &bitangent1, QVector3D &tangent2, QVector3D &bitangent2) {
    QVector3D edge1 = _pos2 - _pos1;
    QVector3D edge2 = _pos3 - _pos1;
    QVector2D deltaUV1 = uv2 - uv1;
    QVector2D deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    tangent1 = {
                f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()),
                f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()),
                f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z())
    };
    tangent1.normalize();

    bitangent1 = {
        f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()),
        f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()),
        f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z())
    };
    bitangent1.normalize();

    // - triangle 2
    edge1 = _pos3 - _pos1;
    edge2 = _pos4 - _pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    tangent2 = {
        f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()),
        f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()),
        f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z())
    };
    tangent2.normalize();

    bitangent2 = {
        f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()),
        f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()),
        f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z())
    };
    bitangent2.normalize();
}

void ImageRectangle::calculate() {
    // 1 2 3 4为左下右上的顺序
    auto x = _pos2 - _pos1;

    float xCount = std::max(x.length() / m_step.x(), 1.f);
    auto y = _pos3 - _pos2;
    float yCount = std::max(y.length() / m_step.y(), 1.f);

    QVector2D uv1 = {0, 0};
    QVector2D uv2 = {xCount, 0};
    QVector2D uv3 = {xCount, yCount};
    QVector2D uv4 = {0, yCount};

    QVector3D tangent1;
    QVector3D tangent2;
    QVector3D bitangent1;
    QVector3D bitangent2;

    getTangent(uv1, uv2, uv3, uv4, tangent1, bitangent1, tangent2, bitangent2);

    auto normal = QVector3D::crossProduct(_pos2 - _pos1, _pos4 - _pos1);
    Buffer cur_rect;
    // 1 2 3
    cur_rect.normal = normal;
    cur_rect.tangent = tangent1;
    cur_rect.bitangent = bitangent1;
    cur_rect.pos = _pos1;
    cur_rect.coord = uv1;
    m_buffer.push_back(cur_rect);

    cur_rect.pos = _pos2;
    cur_rect.coord = uv2;
    m_buffer.push_back(cur_rect);

    cur_rect.pos = _pos3;
    cur_rect.coord = uv3;
    m_buffer.push_back(cur_rect);

    // 1 3 4
    cur_rect.tangent = tangent2;
    cur_rect.bitangent = bitangent2;
    cur_rect.pos = _pos1;
    cur_rect.coord = uv1;
    m_buffer.push_back(cur_rect);

    cur_rect.pos = _pos3;
    cur_rect.coord = uv3;
    m_buffer.push_back(cur_rect);
    cur_rect.pos = _pos4;
    cur_rect.coord = uv4;
    m_buffer.push_back(cur_rect);
}

void ImageRectangle::setImagePath(const QString &path, const QString &diffuseMap, const QString &normalMap, const QString &heightMap) {
    m_image_diffuse = path + diffuseMap;
    m_image_normal = path + normalMap;
    m_image_height = path + heightMap;
}
