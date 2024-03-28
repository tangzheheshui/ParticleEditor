#include "texturemng.h"
#include <QImage>
#include <QOpenGLTexture>

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

TextureMng::TextureMng() {
    createShadowTexture();
}

void TextureMng::loadTexture(const QString &filename) {
    std::lock_guard guard(m_mutux);
    auto iter = m_map_tetures.find(filename);
    if (iter == m_map_tetures.end()) {
        auto texture = std::make_shared<QOpenGLTexture>(QImage(filename));
        m_map_tetures[filename] = texture;
    }
}

SPTR_TEXTURE TextureMng::getTexture(const QString &filename) const {
    std::lock_guard guard(m_mutux);
    auto iter = m_map_tetures.find(filename);
    if (iter != m_map_tetures.end()) {
        return iter->second;
    }
    return nullptr;
}

void TextureMng::createShadowTexture() {
    std::lock_guard guard(m_mutux);
    auto texture = std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D);
    texture->setSize(SHADOW_WIDTH, SHADOW_HEIGHT);
    texture->setFormat(QOpenGLTexture::D24);

    // 设置纹理参数（可选）
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);

    // 分配存储空间
    texture->allocateStorage();
    m_map_tetures[STR_DEPTH_TEXTURE] = texture;
}
