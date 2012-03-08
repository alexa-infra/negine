/**
 * @author  Victor Sukochev <sukochevvv@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
 
#pragma once

#include "base/types.h"
#include "stb/stb_truetype.h"
#include "base/math/vector.h"
#include "renderer/mesh.h"

namespace base {
namespace opengl {
    class Texture;
    class VertexBuffer;
 
    //! Sprite that contains writted text
    class SpriteFont 
    {
    private:
        u32 max_chars_;             //!< maximum text length
        u32 text_length_;           //!< current text length
        VertexBuffer* vbo_;         //!< vertex buffer for drawing text
        Texture* texture_;          //!< font texture
        stbtt_bakedchar* cdata_;    //!< font data: ASCII 32..126 is 95 glyphs
    public:
        SpriteFont(const std::string& filename, f32 height, u32 max_chars);
        ~SpriteFont();

        void SetText(const base::math::Vector2& position, const std::string& text, const math::Vector4& color);
        void Draw(const AttributeBinding& binding);
        Texture* texture() const { return texture_; }

    private:
        DISALLOW_COPY_AND_ASSIGN(SpriteFont);
    };

}//namespace opengl
}//namespace base


