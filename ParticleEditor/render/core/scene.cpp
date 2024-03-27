#include "scene.h"
#include "render/object/drawline.h"
#include <QVector3D>
#include "camera.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int SCR_WIDTH = 512;
const unsigned int SCR_HEIGHT = 384;

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
