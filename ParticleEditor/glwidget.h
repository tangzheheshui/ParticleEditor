// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLFunctions_3_2_Core>
#include <QQuickWindow>
#include <QQuickItem>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QQuickItem, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    GLWidget();
    ~GLWidget();

public slots:
    void sync();
    void cleanup();
private slots:
    void handleWindowChanged(QQuickWindow* win);
    void paintGL();
};

#endif
