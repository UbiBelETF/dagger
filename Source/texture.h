#pragma once

#include <glad.h>

#include <string>
#include <filesystem>
#include <memory>

struct TextureSystem;

class Texture
{
    std::string m_Name;
    int m_Width, m_Height, m_Channels;
    std::filesystem::path m_Path;
    unsigned int m_TextureId;
    std::uint64_t m_Handle;

    unsigned char m_TextureIndex;
    friend struct TextureSystem;
public:

    inline const int Width() const
    {
        return m_Width;
    }

    inline const int Height() const
    {
        return m_Height;
    }

    inline const int Depth() const
    {
        return m_Channels;
    }

    inline const unsigned int TextureId() const
    {
        return m_TextureId;
    }

    inline const std::uint64_t Handle() const
    {
        return m_Handle;
    }

    inline const unsigned char Index() const
    {
        return m_TextureIndex;
    }

    Texture()
        : m_Name{}
        , m_Width{ 0 }
        , m_Height{ 0 }
        , m_Channels{ 0 }
        , m_Path{}
        , m_TextureId{ 0 }
        , m_Handle{ 0 }
        , m_TextureIndex{ 0 }
    {}

    Texture(const std::string name_, const std::filesystem::path path_, unsigned char* data_, int width_, int height_, int channels_)
        : m_Name{ name_ }
        , m_Path{ path_ }
        , m_Width{ width_ }
        , m_Height{ height_ }
        , m_Channels{ channels_ }
        , m_TextureId{ 0 }
        , m_Handle{ 0 }
        , m_TextureIndex{ 0 }
    {
        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
        
        m_Handle = glGetTextureHandleARB(m_TextureId);
        glMakeTextureHandleResidentARB(m_Handle);
        assert(glIsTextureHandleResidentARB(m_Handle));

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    Texture(const Texture& other_)
        : m_Name{ other_.m_Name }
        , m_Path{ other_.m_Path }
        , m_Width{ other_.m_Width }
        , m_Height{ other_.m_Height }
        , m_Channels{ other_.m_Channels }
        , m_TextureId{ other_.m_TextureId }
        , m_Handle{ other_.m_Handle }
        , m_TextureIndex{ other_.m_TextureIndex }
    {}

    Texture& operator=(const Texture& other_)
    {
        return *this;
    }
};