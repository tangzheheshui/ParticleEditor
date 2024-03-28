#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <QOpenGLTexture>
#include <QMatrix4x4>

class BaseDraw;
class QOpenGLShaderProgram;

class Scene {
public:
    static Scene& getScene();
    //static GLuint GetShadowTexture() { return _depthTexture; }
    QMatrix4x4& GetLightVPMatrix();
    void draw();
    void update();
    void setLightUniform(QOpenGLShaderProgram* shader);

private:
    Scene();
    ~Scene() {}
    Scene(const Scene& scene) = delete;
    Scene& operator=(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;
    void createObjs();
    //void loadTexture(const fs::path& dirPath);

private:
    //std::shared_ptr<Line> getTestLine();
    //std::vector<std::shared_ptr<ImageRectangle>> createGlass();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
    GLuint _depthMapFBO = 0;
    //inline static GLuint _depthTexture = 0;
    //inline static Matrix* _lightVPMatrix = nullptr;

    //GLuint _fragment_alpha_buffer;
    //GLuint _atomic_counter;
    //GLuint _texture_blend;
    //GLuint _pbo_head_pointer;
    QMatrix4x4* m_mat_lightVMP = nullptr;
};

#endif // SCENE_H
