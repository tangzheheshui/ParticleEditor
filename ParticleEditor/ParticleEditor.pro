QT += widgets opengl openglwidgets quick quickcontrols2

HEADERS += \
    glwidget.h \
    qmldata.h \
    render/core/camera.h \
    render/core/light.h \
    render/core/renderthread.h \
    render/core/scene.h \
    render/core/shadercache.h \
    render/core/texturemng.h \
    render/object/BaseDraw.h \
    render/object/drawline.h \
    render/object/imagerectangle.h

SOURCES += \
    glwidget.cpp \
    main.cpp \
    qmldata.cpp \
    render/core/camera.cpp \
    render/core/renderthread.cpp \
    render/core/scene.cpp \
    render/core/shadercache.cpp \
    render/core/texturemng.cpp \
    render/object/drawline.cpp \
    render/object/imagerectangle.cpp

DISTFILES +=

RESOURCES += \
    qml.qrc \
    shader.qrc
