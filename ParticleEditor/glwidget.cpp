// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "render/core/scene.h"

GLWidget::GLWidget()
{
    int w = width();
    int h = width();
    connect(this, &QQuickItem::windowChanged, this, &GLWidget::handleWindowChanged);
}

void GLWidget::handleWindowChanged(QQuickWindow* win) {
    int w = width();
    int h = height();
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &GLWidget::sync, Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        //win->setClearBeforeRendering(false);
    }
}

void GLWidget::sync()
{
    connect(window(), &QQuickWindow::afterRendering, this, &GLWidget::paintGL, Qt::DirectConnection);
}

void GLWidget::cleanup() {

}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL() {
    static bool temp = false;
    if (!temp) {
        QSGRendererInterface *rif = window()->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL);
        initializeOpenGLFunctions();
        temp = true;
    }

    int xx = x();
    int yy = y();
    int w = width();
    int h = height();
    yy = window()->height() - yy - h;
    window()->beginExternalCommands();
    glViewport(2 * xx, 2 * yy, 2 * w, 2 * h);

    glEnable(GL_SCISSOR_TEST);
    glScissor(2 * xx, 2 * yy, 2 * w, 2 * h);
    Scene::getScene().draw();
    window()->endExternalCommands();
    //window()->resetOpenGLState();
}
