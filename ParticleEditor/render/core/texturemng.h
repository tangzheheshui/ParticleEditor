#ifndef TEXTUREMNG_H
#define TEXTUREMNG_H

#include <map>
#include <mutex>
#include <QString>

class QOpenGLTexture;

typedef std::shared_ptr<QOpenGLTexture> SPTR_TEXTURE;
static const QString STR_DEPTH_TEXTURE = "depth_texture";

class TextureMng
{
public:
    static TextureMng& getInstance() {
        std::call_once(_callOnce, [&]() { _instance = new TextureMng; });
        return *_instance;
    }

    void loadTexture(const QString &filename);
    SPTR_TEXTURE getTexture(const QString &filename) const;
private:
    TextureMng();
    ~TextureMng() {}
    TextureMng(const TextureMng& mng) = delete;
    TextureMng& operator = (const TextureMng& mng) = delete;
    TextureMng(TextureMng&& mng) = delete;
    TextureMng& operator = (TextureMng&& mng) = delete;
private:
    void createShadowTexture();
private:
    inline static TextureMng* _instance = nullptr;
    inline static std::once_flag _callOnce;
    std::map<QString, SPTR_TEXTURE> m_map_tetures;
    mutable std::mutex m_mutux;
};

#endif // TEXTUREMNG_H
