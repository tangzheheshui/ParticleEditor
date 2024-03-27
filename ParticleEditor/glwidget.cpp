// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "render/core/scene.h"

GLWidget::GLWidget(const QColor &background)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    //initializeOpenGLFunctions();
}

void GLWidget::paintGL()
{
    //Scene::getScene().update();
    Scene::getScene().draw();
}
