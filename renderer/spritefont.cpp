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
#include "renderer/gltexture.h"

namespace base {
namespace opengl {

SpriteFont::SpriteFont(const std::string& filename, f32 height)
    : textVBO_(NULL)
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

    base::opengl::TextureInfo tex_info;
    tex_info.MinFilter = base::opengl::TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    tex_info.Height = 512;
    tex_info.Width = 512;
    tex_info.Pixel = PixelTypes::Gray;

    texture_ = new base::opengl::Texture;
    texture_->GenerateFromBuffer(tex_info, temp_bitmap);
}

void SpriteFont::setText(f32 x, f32 y, const std::string& str)
{
    delete textVBO_;

    const i32 length = str.size();
    u32 vertex_index_ = 0;
    u32 face_index_ = 0;
    
    //allocating memory for vertex data 
    Vertex* vertexes_ = new Vertex[length*4];
    Face* faces_ = new Face[length*2];

    Vector3 norm(0.0, 0.0, 1.0);

    for (u32 i=0; i<length; i++) {
        u8 ch = str[i];
            
        if (ch < 32 || ch > 128)
            continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata_, 
            512,
            512, 
            ch-32,
            &x,
            &y,
            &q,
            1);//1=opengl,0=old d3d

        //VERTEX 0
        vertexes_[vertex_index_].pos = Vector3(q.x0+x, q.y0+y, 0.0);
        vertexes_[vertex_index_].n = norm;
        vertexes_[vertex_index_].tex = Vector2(q.s0, q.t1);

        //VERTEX 1
        vertexes_[vertex_index_+1].pos = Vector3(q.x1+x, q.y0+y, 0.0);
        vertexes_[vertex_index_+1].n = norm;
        vertexes_[vertex_index_+1].tex = Vector2(q.s1, q.t1);

        //VERTEX 2
        vertexes_[vertex_index_+2].pos = Vector3(q.x1+x, q.y1+y, 0.0);
        vertexes_[vertex_index_+2].n = norm;
        vertexes_[vertex_index_+2].tex = Vector2(q.s1, q.t0);

        //VERTEX 3
        vertexes_[vertex_index_+3].pos = Vector3(q.x0+x, q.y1+y, 0.0);
        vertexes_[vertex_index_+3].n = norm;
        vertexes_[vertex_index_+3].tex = Vector2(q.s0, q.t0);

        //indexes
        faces_[face_index_].index[0] = vertex_index_;
        faces_[face_index_].index[1] = vertex_index_+1;
        faces_[face_index_].index[2] = vertex_index_+2;

        faces_[face_index_+1].index[0] = vertex_index_+2;
        faces_[face_index_+1].index[1] = vertex_index_+3;
        faces_[face_index_+1].index[2] = vertex_index_;

        faces_[face_index_].n = norm;
        faces_[face_index_+1].n = norm;

        face_index_ += 2;
        vertex_index_ += 4;
    }
   
    textVBO_ = new VertexBuffer(vertexes_, vertex_index_, faces_, face_index_);
    delete[] vertexes_;
    delete[] faces_;   
}

void SpriteFont::Draw (const AttributeBinding& binding)
{
    texture_->Bind();
    if (textVBO_ != NULL)
        textVBO_ ->Draw(binding);
}

SpriteFont::~SpriteFont()
{
    delete[] cdata_;
    delete textVBO_;
    delete texture_;
}

}//namespace opengl
}//namespace base