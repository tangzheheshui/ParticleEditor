#include "scene.h"
#include "render/object/drawline.h"
#include "render/core/light.h"
#include <QOpenGLShader>
#include <QVector3D>
#include <QDir>
#include "camera.h"
#include "render/object/imagerectangle.h"
#include "render/core/texturemng.h"

Scene& Scene::getScene() {
    static Scene instance;
    return instance;
}

Scene::Scene() {
    loadTexture(":/render/res/textures");
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

    // 地面
    {
        std::shared_ptr<ImageRectangle> objGround = std::make_shared<ImageRectangle>();
        float ground_width = 10;
        objGround->setImagePath(":/render/res/textures/", "bricks2.jpg", "bricks2_normal.jpg", "bricks2_disp.jpg");

        objGround->setSetp(5, 5);
        QVector3D p1(-ground_width, 0,  ground_width);
        QVector3D p2(ground_width, 0,  ground_width);
        QVector3D p3(ground_width, 0, -ground_width);
        QVector3D p4(-ground_width, 0, -ground_width);
        objGround->setPoints(p1, p2, p3, p4);
        objGround->calculate();
        m_vec_drawobj.push_back(objGround);
    }
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

void Scene::loadTexture(const QString &dirPath) {
    QDir directory(dirPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.hdr";
    directory.setNameFilters(filters);
    QStringList pngFiles = directory.entryList(QDir::Files);

    foreach (const QString &file, pngFiles) {
        // todo 使用异步任务队列
        TextureMng::getInstance().loadTexture(directory.filePath(file));
        qDebug() << directory.filePath(file);
    }

    QStringList subDirs = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDir, subDirs) {
        loadTexture(directory.filePath(subDir));
    }
}
