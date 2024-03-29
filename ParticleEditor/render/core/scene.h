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
    void setViewport(const QVector4D &port);
private:
    Scene();
    ~Scene() {}
    Scene(const Scene& scene) = delete;
    Scene& operator=(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;
    void createObjs();
    void loadTexture(const QString &dirPath);

private:
    //std::shared_ptr<Line> getTestLine();
    //std::vector<std::shared_ptr<ImageRectangle>> createGlass();
private:
    std::vector<std::shared_ptr<BaseDraw>> m_vec_drawobj;
    QMatrix4x4* m_mat_lightVMP = nullptr;
    QVector4D m_viewport;
};

#endif // SCENE_H
