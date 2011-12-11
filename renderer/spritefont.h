/**
 * @author  Victor Sukochev <sukochevvv@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
 
#pragma once

#include "base/types.h"
#include "renderer/vertexbuffer.h"
#include "renderer/sprite.h"

#include "stb/stb_truetype.h"

namespace base {
namespace opengl {
    class Texture;

    //! Sprite that contains writted text
    class SpriteFont 
    {
    private:
        u32 max_chars_;             //!< maximum text length
        VertexBuffer* vbo_;         //!< vertex buffer for drawing text
        Texture* texture_;          //!< font texture
        stbtt_bakedchar* cdata_;    //!< font data: ASCII 32..126 is 95 glyphs
        std::string text_;          //!< current displayed text
        Vector2 position_;          //!< position of displayed text
    public:
        SpriteFont(const std::string& filename, f32 height, u32 max_chars);
        ~SpriteFont();

        void SetText(f32 x, f32 y, const std::string& text);
        void Draw(const AttributeBinding& binding);
    };

}//namespace opengl
}//namespace base


