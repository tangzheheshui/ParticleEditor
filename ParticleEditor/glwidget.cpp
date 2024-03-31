// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include <QSGSimpleTextureNode>
#include "render/core/camera.h"
#include "render/core/renderthread.h"
#include "render/core/texturemng.h"
#include "render/core/shadercache.h"
#include <QSGFlatColorMaterial>

GLWidget::GLWidget()
{
    _node = new QSGSimpleTextureNode;

    setFlag(ItemHasContents, true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptTouchEvents(true);
    setFlag(ItemAcceptsInputMethod, true);
    connect(this, &QQuickItem::windowChanged, this, &GLWidget::handleWindowChanged);

    _timer.setInterval(20);
    QObject::connect(&_timer, &QTimer::timeout, [this](){
        this->update();
    });
    _timer.start();
}

void GLWidget::handleWindowChanged(QQuickWindow* win) {
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &GLWidget::sync, Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
    }
}

void GLWidget::sync()
{
    //connect(window(), &QQuickWindow::afterRendering, this, &GLWidget::paintGL, Qt::DirectConnection);
}

void GLWidget::cleanup() {

}

GLWidget::~GLWidget()
{
    if (_renderThread) {
        delete _renderThread;
        _renderThread = nullptr;
    }

    if (_node) {
        delete _node;
        _node = nullptr;
    }

    if (_renderThread) {
        delete _renderThread;
        _renderThread = nullptr;
    }
}

QSGNode* GLWidget::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    if (!_renderThread) {
        // 在切换上下文前，将一些可以共享的gl对象创建出来
        TextureMng::getInstance();
        ShaderCache::GetInstance();

        // 将主线程的上下文，share给各个线程
        auto context = QOpenGLContext::currentContext();
        auto mainSurface = context->surface();

        context->doneCurrent();
        _renderThread = new RenderThread(context, {(int)width(), (int)height()},this);
        context->makeCurrent(mainSurface);

        _renderThread->start();
        connect(_renderThread, &RenderThread::textureReady, this, &GLWidget::handleTextureReady);
    }

    return _node;
}

void GLWidget::handleTextureReady(int textureId, QSize size) {
    QImage image(size.width(), size.height(), QImage::Format_RGBA8888);
    glBindTexture(GL_TEXTURE_2D, textureId);

    _renderThread->lock();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    _renderThread->unlock();

    image = image.mirrored(false, true);
    auto img = window()->createTextureFromImage(image);
    _node->setTexture(img);
    _node->setRect(boundingRect());
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    _posPressMove = _posPress = event->position();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    auto curPos = event->position();
    auto offset = curPos - _posPressMove;

    float sensitivity = 0.5; // change this value to your liking
    offset *= sensitivity;

    float yaw = Camera::GetCamera().getYaw();
    yaw -= offset.x();
    float pitch = Camera::GetCamera().getPitch();
    pitch += offset.y();

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < 1.0f)
        pitch = 1.0f;

    qDebug() << "_yaw = " << yaw << ", _pitch = " << pitch;
    Camera::GetCamera().setPitch(pitch);
    Camera::GetCamera().setYaw(yaw);

    _posPressMove = event->position();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "mouseReleaseEvent at: " << event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    if (event->isUpdateEvent()) {
        float fov = Camera::GetCamera().getFov();
        float y = event->pixelDelta().y();
        y /= 2;
        fov += y;
        if (fov < 3.0f)
            fov = 3.0f;
        if (fov > 90.0f)
            fov = 90.0f;

        Camera::GetCamera().setFov(fov);
    }
}


