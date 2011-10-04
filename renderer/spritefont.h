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

    class SpriteFont 
    {
    public:
        SpriteFont(const std::string& filename, f32 height);
        ~SpriteFont();

        void setText (f32 x, f32 y, const std::string& text); 
        void Draw(const AttributeBinding& binding);

    private:
        base::opengl::Texture* texture_;

        stbtt_bakedchar* cdata_; // ASCII 32..126 is 95 glyphs

        VertexBuffer *textVBO_;
    };

}//namespace opengl
}//namespace base


