// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLFunctions_3_2_Core>
#include <QQuickWindow>
#include <QQuickItem>
#include <QTimer>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QQuickItem, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    GLWidget();
    ~GLWidget();

protected:
    virtual void focusInEvent(QFocusEvent *) override;
    virtual void focusOutEvent(QFocusEvent *) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void touchEvent(QTouchEvent *event) override;
    virtual void hoverEnterEvent(QHoverEvent *event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
    virtual void hoverLeaveEvent(QHoverEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *) override;
    virtual void dragMoveEvent(QDragMoveEvent *) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *) override;
    virtual void dropEvent(QDropEvent *) override;

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
public slots:
    void sync();
    void cleanup();
private slots:
    void handleWindowChanged(QQuickWindow* win);
    void paintGL();
private:
    QPointF _posPress;
    QPointF _posPressMove;
    QTimer _timer;
};

#endif
