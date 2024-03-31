#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QOpenGLFunctions_4_1_Core>
#include <QThread>
#include <QSize>
#include <QOffscreenSurface>
#include <QOpenGLFunctions>

// OpenGL 渲染线程
class RenderThread : public QThread, protected QOpenGLFunctions_4_1_Core {
    Q_OBJECT
public:
    RenderThread(QOpenGLContext* context, QSize size, QObject *parent = nullptr);
    ~RenderThread() {
        glDeleteTextures(1, &m_textureId);

        quit();
        wait(); // 等待线程退出
    }
    void lock() { _mutexTexture.lock(); }
    void unlock() { _mutexTexture.unlock(); }
protected:
    void run() override;
private:
    void createTexture();
signals:
    void textureReady(int textureId, QSize size);
private:
    QSize m_size;
    GLuint m_textureId = 0;
    GLuint m_FBO = 0;
    GLuint m_RBO = 0;
    QOpenGLContext _context;
    QOffscreenSurface* _surface = nullptr;
    std::mutex _mutexTexture;
};

#endif // RENDERTHREAD_H
