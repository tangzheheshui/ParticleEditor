// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QSurfaceFormat>
#include "glwidget.h"
#include <QtQuick/QQuickView>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSettings>

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 1); // 设置OpenGL版本为3.2
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    qmlRegisterType<GLWidget>("CustomOpenGL",1,0,"GLWidget");

    QQuickStyle::setStyle("Fusion");
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    int w = view.width();
    int h = view.height();
    view.setMinimumHeight(h);
    view.setMaximumHeight(h);
    view.setMinimumWidth(w);
    view.setMaximumWidth(w);

    return a.exec();
}
