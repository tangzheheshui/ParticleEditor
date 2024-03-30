#include "renderthread.h"
#include "scene.h"
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOffscreenSurface>

RenderThread::RenderThread(QOpenGLContext* context, QSize size, QObject *parent)
    : QThread(parent),
    m_size(size)
{
    _surface = new QOffscreenSurface(nullptr, parent);
    _surface->setFormat(context->format());
    _surface->create();

    _context.setFormat(context->format());
    _context.setShareContext(context);
    _context.create();
    _context.moveToThread(this);

}

void RenderThread::createTexture() {
    QOpenGLFunctions *f = _context.functions();
    initializeOpenGLFunctions();

    // framebuffer
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // texture
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_size.width(), m_size.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

    // rbo
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size.width(), m_size.height()); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO); // now actually attach it
}

void RenderThread::run() {
    while (1) {
        _context.makeCurrent(_surface);
        if (m_FBO == 0) {
            createTexture();
        }
        // 将当前线程的 OpenGL 上下文设为当前上下文

        QOpenGLFunctions *f = _context.functions();

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        // glViewport(0, 0, m_size.width(), m_size.height());

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        Scene::getScene().setViewport({0, 0, (float)m_size.width(), (float)m_size.height()});

        Scene::getScene().draw();

        QImage image(m_size.width(), m_size.height(), QImage::Format_RGBA8888);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

        // 将图像保存到文件
        QString filePath = "/Users/liuhaifeng/personal/GameEditor/ParticleEditor/screen_picture/test.png";
        if (image.save(filePath)) {
            //qDebug() << "Texture saved to" << filePath;
        }

        // 发送渲染完成的纹理信号
        emit textureReady(m_textureId, m_size);

        // 释放上下文
        _context.doneCurrent();
    }
}
