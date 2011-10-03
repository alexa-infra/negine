/**
 * @author  Victor Sukochev <sukochevvv@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/

#include "renderer/spritefont.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#define STBI_HEADER_FILE_ONLY
#include "stb/stb_image.c"
#include <iostream>
#include "base/stream.h"

namespace base {
namespace opengl {

SpriteFont::SpriteFont(const std::string& filename, f32 height)
    : height_(height)
    , valid_(false)
    , vertexes_(NULL)
    , faces_(NULL)
    , textVBO_(NULL)
{
    base::FileBinary file(filename.c_str());
    u32 size = file.size();
    u8* ttf_buffer = new u8[size];
    file.read(ttf_buffer, size);

    u8 temp_bitmap[512*512];

    cdata_ = new stbtt_bakedchar[96];
    stbtt_BakeFontBitmap((const u8*)ttf_buffer,
        0,
        32.0,
        temp_bitmap,
        512,
        512, 
        32,
        96,
        (stbtt_bakedchar*)cdata_); // no guarantee this fits!   
    delete[] ttf_buffer;

    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_LUMINANCE,
        512,
        512,
        0,
        GL_LUMINANCE,
        GL_UNSIGNED_BYTE,
        temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
}

void SpriteFont::setText (f32 x, f32 y, const std::string& str)
{
    delete textVBO_;

    i32 length = str.size();
    vertex_index_ = 0;
    face_index_ = 0;
    
    //allocating memory for vertex data 
    vertexes_ = new Vertex[length*4];
    faces_ = new Face[length*2];

    const char* text = str.c_str();
    while (length) {
        if (*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata_, 
                512,
                512, 
                *text-32,
                &x,
                &y,
                &q,
                1);//1=opengl,0=old d3d

            //VERTEX 0
            vertexes_[vertex_index_].pos.x = q.x0+x;
            vertexes_[vertex_index_].pos.y = q.y0+y;
            vertexes_[vertex_index_].pos.z = 0.0;
            vertexes_[vertex_index_].n.x = 0.0;
            vertexes_[vertex_index_].n.y = 0.0;
            vertexes_[vertex_index_].n.z = 1.0;
            vertexes_[vertex_index_].tex.x = q.s0;
            vertexes_[vertex_index_].tex.y = q.t1;
            //VERTEX 1
            vertexes_[vertex_index_+1].pos.x = q.x1+x;
            vertexes_[vertex_index_+1].pos.y = q.y0+y;
            vertexes_[vertex_index_+1].pos.z = 0.0;
            vertexes_[vertex_index_+1].n.x = 0.0;
            vertexes_[vertex_index_+1].n.y = 0.0;
            vertexes_[vertex_index_+1].n.z = 1.0;
            vertexes_[vertex_index_+1].tex.x = q.s1;
            vertexes_[vertex_index_+1].tex.y = q.t1;
            //VERTEX 2
            vertexes_[vertex_index_+2].pos.x = q.x1+x;
            vertexes_[vertex_index_+2].pos.y = q.y1+y;
            vertexes_[vertex_index_+2].pos.z = 0.0;
            vertexes_[vertex_index_+2].n.x = 0.0;
            vertexes_[vertex_index_+2].n.y = 0.0;
            vertexes_[vertex_index_+2].n.z = 1.0;
            vertexes_[vertex_index_+2].tex.x = q.s1;
            vertexes_[vertex_index_+2].tex.y = q.t0;
            //VERTEX 3
            vertexes_[vertex_index_+3].pos.x = q.x0+x;
            vertexes_[vertex_index_+3].pos.y = q.y1+y;
            vertexes_[vertex_index_+3].pos.z = 0.0;
            vertexes_[vertex_index_+3].n.x = 0.0;
            vertexes_[vertex_index_+3].n.y = 0.0;
            vertexes_[vertex_index_+3].n.z = 1.0;
            vertexes_[vertex_index_+3].tex.x = q.s0;
            vertexes_[vertex_index_+3].tex.y = q.t0;
            //indexes
            faces_[face_index_].index[0] = vertex_index_;
            faces_[face_index_].index[1] = vertex_index_+1;
            faces_[face_index_].index[2] = vertex_index_+2;

            faces_[face_index_+1].index[0] = vertex_index_+2;
            faces_[face_index_+1].index[1] = vertex_index_+3;
            faces_[face_index_+1].index[2] = vertex_index_;

            faces_[face_index_].n.x = 0.0;
            faces_[face_index_].n.y = 0.0;
            faces_[face_index_].n.z = 1.0;

            faces_[face_index_+1].n.x = 0.0;
            faces_[face_index_+1].n.y = 0.0;
            faces_[face_index_+1].n.z = 1.0;

            face_index_ += 2;
        }

        ++text;
        --length;
        vertex_index_+=4;
    }
   
    textVBO_ = new VertexBuffer(vertexes_, vertex_index_, faces_, face_index_);   
}

void SpriteFont::Draw (const AttributeBinding& binding)
{
    glBindTexture(GL_TEXTURE_2D, tex_);
    if (textVBO_ != NULL)
        textVBO_ ->Draw(binding);
}

SpriteFont::~SpriteFont()
{
    delete[] cdata_;
    delete textVBO_;
}

}//namespace opengl
}//namespace base