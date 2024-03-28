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

class QOpenGLShaderProgram;
typedef std::shared_ptr<QOpenGLShaderProgram> SPTR_SHADER_PROGRAM;

class ShaderCache {
public:
    static ShaderCache& GetInstance();
    SPTR_SHADER_PROGRAM GetShader(ShaderType type);
private:
    ShaderCache();
    ~ShaderCache() {}
    ShaderCache(const ShaderCache& shader) = delete;
    ShaderCache(ShaderCache&& shader) = delete;
    ShaderCache& operator=(const ShaderCache& shader) = delete;
    ShaderCache& operator=(ShaderCache&& shader) = delete;
private:
    SPTR_SHADER_PROGRAM addShader(const QString& vert, const QString& frag);
private:
    std::map<ShaderType, SPTR_SHADER_PROGRAM> m_map_shader;
};

#endif // SHADERCACHE_H
