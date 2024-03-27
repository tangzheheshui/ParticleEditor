// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include "glwidget.h"

MainWindow::MainWindow()
{
    GLWidget *glwidget = new GLWidget(qRgb(20, 20, 50));
    setCentralWidget(glwidget);
}
