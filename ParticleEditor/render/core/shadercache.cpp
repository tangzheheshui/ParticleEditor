#include "shadercache.h"
#include <QDir>
#include <QUrl>
const QString file_root = "../ren";

ShaderCache& ShaderCache::GetInstance() {
    static ShaderCache instance;
    return instance;
}

ShaderCache::ShaderCache() {
    QString rootPath = ":/render/shader/";
    // 线
    QOpenGLShader m_vshader(QOpenGLShader::Vertex);
    QString filename = rootPath + "vert_color.vert";
    bool ret = m_vshader.compileSourceFile(filename);
    assert(ret);

    filename = rootPath + "frag_color.frag";
    QOpenGLShader m_fshader(QOpenGLShader::Fragment);
    ret = m_fshader.compileSourceFile(filename);
    assert(ret);

    auto m_program = new QOpenGLShaderProgram;
    ret = m_program->addShader(&m_vshader);
    assert(ret);

    ret = m_program->addShader(&m_fshader);
    assert(ret);

    ret = m_program->link();
    assert(ret);

    auto pair = std::make_pair(ShaderType::Color, m_program);
    m_map_shader.insert(pair);
}

QOpenGLShaderProgram* ShaderCache::GetShader(ShaderType type) {
    auto iter = m_map_shader.find(type);
    if (iter != m_map_shader.end()) {
        return iter->second;
    }
    return nullptr;
}
