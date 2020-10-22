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
    UInt64 m_Handle;
    Float32 m_Ratio;
    Float32 m_TextureIndex;
    
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

    inline const UInt64 Handle() const
    {
        return m_Handle;
    }

    inline const Float32 Ratio() const
    {
        return m_Ratio;
    }

    inline const Float32 Index() const
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
        , m_Ratio{ 0 } 
    {}

    Texture(String name_, const FilePath path_, UInt8* data_, UInt32 width_, UInt32 height_, UInt32 channels_)
        : m_Name{ name_ }
        , m_Path{ path_ }
        , m_Width{ width_ }
        , m_Height{ height_ }
        , m_Channels{ channels_ }
        , m_TextureId{ 0 }
        , m_Handle{ 0 }
        , m_TextureIndex{ 0 }
        , m_Ratio{ (Float32)height_ / (Float32)width_ }
    {
        assert(m_Ratio > 0);

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
        , m_Ratio{ other_.m_Ratio }
    {}

    Texture& operator=(const Texture& other_)
    {
        return *this;
    }
};