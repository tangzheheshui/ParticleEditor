#include "shadercache.h"
#include <QDir>
#include <QUrl>

ShaderCache& ShaderCache::GetInstance() {
    static ShaderCache instance;
    return instance;
}

SPTR_SHADER_PROGRAM ShaderCache::addShader(const QString& vert, const QString& frag) {
    QString rootPath = ":/render/shader/";

    QOpenGLShader m_vshader(QOpenGLShader::Vertex);
    if (!m_vshader.compileSourceFile(rootPath + vert)) {
        return nullptr;
    }

    QOpenGLShader m_fshader(QOpenGLShader::Fragment);
    if (!m_fshader.compileSourceFile(rootPath + frag)) {
        return nullptr;
    }

    auto shader = std::make_shared<QOpenGLShaderProgram>();
    if (shader->addShader(&m_vshader) &&
        shader->addShader(&m_fshader) &&
        shader->link()) {
        return shader;
    }

    return nullptr;
}

ShaderCache::ShaderCache() {
    // 线
    auto shader = addShader("color.vert", "color.frag");
    assert(shader);
    m_map_shader.insert(std::make_pair(ShaderType::Color, shader));

    // 地面
    shader = addShader("ground.vert", "ground.frag");
    assert(shader);
    m_map_shader.insert(std::make_pair(ShaderType::Ground, shader));
}

SPTR_SHADER_PROGRAM ShaderCache::GetShader(ShaderType type) {
    auto iter = m_map_shader.find(type);
    if (iter != m_map_shader.end()) {
        return iter->second;
    }
    return nullptr;
}
