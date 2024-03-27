#ifndef CUSTOMRENDER_H
#define CUSTOMRENDER_H

#include <QtQuick/QQuickFramebufferObject>


class CustomFramebufferObject : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    CustomFramebufferObject(QQuickItem *parent = nullptr);
    QQuickFramebufferObject::Renderer *createRenderer() const override;
};

// render
class CustomRenderer : public QQuickFramebufferObject::Renderer
{
public:
    CustomRenderer() {
        // 在这里初始化 OpenGL 相关内容
        int a = 0;
        int b = a+1;
    }

    void synchronize(QQuickFramebufferObject *item) override {
        // 在渲染之前同步数据
        int a = 0;
    }

    void render() override {
        // 执行实际的 OpenGL 渲染
        int a = 0;
    }
};

#endif // CUSTOMRENDER_H
