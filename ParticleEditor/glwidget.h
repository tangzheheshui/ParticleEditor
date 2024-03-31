// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QQuickWindow>
#include <QQuickItem>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class RenderThread;
class QSGSimpleTextureNode;

class GLWidget : public QQuickItem
{
    Q_OBJECT
public:
    GLWidget();
    ~GLWidget();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
public slots:
    void sync();
    void cleanup();
private slots:
    void handleWindowChanged(QQuickWindow* win);
    void handleTextureReady(int textureId, QSize size);
private:
    QPointF _posPress;
    QPointF _posPressMove;
    QTimer _timer;
    RenderThread *_renderThread = nullptr;
    QSGSimpleTextureNode *_node = nullptr;
};

#endif
