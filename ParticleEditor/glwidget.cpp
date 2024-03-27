// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "render/core/scene.h"

GLWidget::GLWidget()
{
    connect(this, &QQuickItem::windowChanged, this, &GLWidget::handleWindowChanged);
}

void GLWidget::handleWindowChanged(QQuickWindow* win) {
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &GLWidget::sync, Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
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

    window()->beginExternalCommands();
    Scene::getScene().draw();
    window()->endExternalCommands();
}
