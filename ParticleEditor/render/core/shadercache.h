#ifndef SHADERCACHE_H
#define SHADERCACHE_H

#include <QOpenGLShader>
#include <map>

enum class ShaderType {
    Invalid = -1,
    Image,
    Color,
    Model_Color,
    Model_Texture,
    Shadow_Color,
    Shadow_Texture,
    Debug_DeepTexture,
    Ground,
    Model_Color_Anim,
    Model_Texture_Anim,
};

class ShaderCache {
public:
    static ShaderCache& GetInstance();
    QOpenGLShaderProgram* GetShader(ShaderType type);
private:
    ShaderCache();
private:
    std::map<ShaderType, QOpenGLShaderProgram*> m_map_shader;
};

#endif // SHADERCACHE_H
