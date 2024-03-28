#include "scene.h"
#include "render/object/drawline.h"
#include "render/core/light.h"
#include <QOpenGLShader>
#include <QVector3D>
#include "camera.h"

Scene& Scene::getScene() {
    static Scene instance;
    return instance;
}

Scene::Scene() {
    createObjs();
    Camera::GetCamera().setPosition({0,0,20});
}

void Scene::createObjs() {
    // 绘制坐标轴
    std::vector<unsigned int> indices = {0, 1};

    float len = 15;

    QVector3D pZero = {0, 0, 0};
    QVector3D pX = {len, 0, 0};
    QVector3D pY = {0, len, 0};
    QVector3D pZ = {0, 0, len};
    // 3个轴
    std::shared_ptr<DrawLine> line_x = std::make_shared<DrawLine>();
    std::shared_ptr<DrawLine> line_y = std::make_shared<DrawLine>();
    std::shared_ptr<DrawLine> line_z = std::make_shared<DrawLine>();
    line_x->setData({pZero, pX}, indices);
    line_y->setData({pZero, pY}, indices);
    line_z->setData({pZero, pZ}, indices);

    // 颜色
    line_x->setColor({1, 0, 0});
    line_y->setColor({0, 1, 0});
    line_z->setColor({0, 0, 1});

    m_vec_drawobj.push_back(line_x);
    m_vec_drawobj.push_back(line_y);
    m_vec_drawobj.push_back(line_z);
}

void Scene::update() {
    for (auto obj : m_vec_drawobj) {
        obj->update();
    }
}

void Scene::draw() {
    //glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glDisable(GL_BLEND);
    for (auto obj : m_vec_drawobj) {
        obj->draw();
    }
}

void Scene::setLightUniform(QOpenGLShaderProgram* shader) {
    auto light = Light::GlobalLight();
    shader->setUniformValue("uLight.position", QVector4D(light.position, 1));
    shader->setUniformValue("uLight.direction", light.direction);
    shader->setUniformValue("uLight.cosTheta", light.cosTheta);
    shader->setUniformValue("uLight.ambient", light.ambient);
    shader->setUniformValue("uLight.diffuse", light.diffuse);
    shader->setUniformValue("uLight.specular", light.specular);
}

QMatrix4x4& Scene::GetLightVPMatrix() {
    if (m_mat_lightVMP) {
        return *m_mat_lightVMP;
    }

    QMatrix4x4 lightProjection, lightView;
    auto lightPos = Light::GlobalLight().position;
    float near_plane = 1.0f, far_plane = 15.f;
    lightView.lookAt(lightPos, QVector3D(0.f, 0.f, 0.f), QVector3D(0.0000001, 1, 0.0));
    lightProjection.perspective(qDegreesToRadians(90), 1.f, near_plane, far_plane);
    m_mat_lightVMP = new QMatrix4x4(lightProjection * lightView);
    return *m_mat_lightVMP;
}
