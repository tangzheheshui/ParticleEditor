#include "customrender.h"

CustomFramebufferObject::CustomFramebufferObject(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
{
    //上下翻转，这样就和OpenGL的坐标系一致了
    //setMirrorVertically(true);
    int a = 0;
}

QQuickFramebufferObject::Renderer *CustomFramebufferObject::createRenderer() const
{
    //Renderer 和 FBO 都是内部管理的内存
    return new CustomRenderer();
}
