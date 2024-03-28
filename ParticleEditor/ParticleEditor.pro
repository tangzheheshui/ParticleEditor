QT += widgets opengl openglwidgets quick

HEADERS += \
    glwidget.h \
    render/core/camera.h \
    render/core/light.h \
    render/core/scene.h \
    render/core/shadercache.h \
    render/core/texturemng.h \
    render/object/BaseDraw.h \
    render/object/drawline.h \
    render/object/imagerectangle.h

SOURCES += \
    glwidget.cpp \
    main.cpp \
    render/core/camera.cpp \
    render/core/scene.cpp \
    render/core/shadercache.cpp \
    render/core/texturemng.cpp \
    render/object/drawline.cpp \
    render/object/imagerectangle.cpp

DISTFILES += \
    render/shader/frag_color.frag \
    render/shader/vert_color.vert

RESOURCES += \
    res.qrc
