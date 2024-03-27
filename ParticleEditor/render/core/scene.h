#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <QOpenGLTexture>

class BaseDraw;

class Scene {
private:
    Scene();
    void createObjs();
    //void loadTexture(const fs::path& dirPath);
public:
    static Scene& getScene();
    //static GLuint GetShadowTexture() { return _depthTexture; }
    //static Matrix GetLightVPMatrix();
    void draw();
    void update();
    //void setLightUniform(Shader* shader);
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
};

#endif // SCENE_H
