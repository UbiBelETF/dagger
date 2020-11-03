#pragma once

#include "core/core.h"

#include <glad/glad.h>

#include <filesystem>
#include <memory>

struct TextureSystem;

class Texture
{
    String m_Name;
    UInt32 m_Width, m_Height, m_Channels;
    FilePath m_Path;
    UInt32 m_TextureId;
    Float32 m_Ratio;

    friend struct TextureSystem;

public:

    inline const UInt32 Width() const
    {
        return m_Width;
    }

    inline const UInt32 Height() const
    {
        return m_Height;
    }

    inline const UInt32 Depth() const
    {
        return m_Channels;
    }

    inline const UInt32 TextureId() const
    {
        return m_TextureId;
    }

    inline const Float32 Ratio() const
    {
        return m_Ratio;
    }

    Texture()
        : m_Name{}
        , m_Width{ 0 }
        , m_Height{ 0 }
        , m_Channels{ 0 }
        , m_Path{}
        , m_TextureId{ 0 }
        , m_Ratio{ 0 } 
    {}

    Texture(String name_, const FilePath path_, UInt8* data_, UInt32 width_, UInt32 height_, UInt32 channels_)
        : m_Name{ name_ }
        , m_Path{ path_ }
        , m_Width{ width_ }
        , m_Height{ height_ }
        , m_Channels{ channels_ }
        , m_TextureId{ 0 }
        , m_Ratio{ (Float32)height_ / (Float32)width_ }
    {
        assert(m_Ratio > 0);

        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);

        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, channels_ == 4 ? GL_RGBA : GL_RGB, m_Width, m_Height, 0, channels_ == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data_);
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    Texture(const Texture& other_)
        : m_Name{ other_.m_Name }
        , m_Path{ other_.m_Path }
        , m_Width{ other_.m_Width }
        , m_Height{ other_.m_Height }
        , m_Channels{ other_.m_Channels }
        , m_TextureId{ other_.m_TextureId }
        , m_Ratio{ other_.m_Ratio }
    {}

    Texture& operator=(const Texture&)
    {
        return *this;
    }
};