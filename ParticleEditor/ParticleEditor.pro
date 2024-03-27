QT += widgets opengl openglwidgets quick

HEADERS += \
    glwidget.h \
    render/core/camera.h \
    render/core/scene.h \
    render/core/shadercache.h \
    render/object/BaseDraw.h \
    render/object/drawline.h \

SOURCES += \
    glwidget.cpp \
    main.cpp \
    render/core/camera.cpp \
    render/core/scene.cpp \
    render/core/shadercache.cpp \
    render/object/drawline.cpp \

DISTFILES += \
    render/shader/frag_color.frag \
    render/shader/vert_color.vert

RESOURCES += \
    res.qrc
