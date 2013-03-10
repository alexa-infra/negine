/**
 * \file
 * \author      Victor Sukochev <sukochevvv@gmail.com>
 *              Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
/*
#include "render/spritefont.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#define STBI_HEADER_FILE_ONLY
#include "stb/stb_image.c"
#include <iostream>
#include "base/stream.h"
#include "render/texture.h"
#include "render/vertexbuffer.h"

using base::math::Vector2;
using base::math::Vector3;
using base::math::Vector4;

namespace base
{
namespace opengl
{

SpriteFont::SpriteFont( const std::string& filename, f32 height, u32 max_chars )
    : max_chars_( max_chars )
    , vbo_( NULL )
{
    font_height_ = height;
    base::FileBinary file( filename.c_str() );
    u32 size = file.size();
    u8* ttf_buffer = new u8[size];
    file.read( ttf_buffer, size );
    u8 temp_bitmap[512 * 512];
    cdata_ = new stbtt_bakedchar[96];
    stbtt_BakeFontBitmap( ( const u8* )ttf_buffer,
                          0,
                          font_height_,
                          temp_bitmap,
                          512,
                          512,
                          32,
                          96,
                          ( stbtt_bakedchar* )cdata_ ); // no guarantee this fits!
    delete[] ttf_buffer;
    TextureInfo tex_info;
    tex_info.MinFilter = TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    tex_info.Height = 512;
    tex_info.Width = 512;
    tex_info.Pixel = PixelTypes::Alpha;
    texture_ = new Texture;
    texture_->GenerateFromBuffer( tex_info, temp_bitmap );
    u32 vertex_index_ = 0;
    u32 face_index_ = 0;
    //allocating memory for vertex data
    Vertex* vertexes_ = new Vertex[max_chars_ * 4];
    Face* faces_ = new Face[max_chars_ * 2];
    Vector4 color( 1.0, 0.0, 0.0, 1.0 );

    for ( u32 i = 0; i < max_chars_; i++ ) {
        vertexes_[vertex_index_].color = color;
        vertexes_[vertex_index_ + 1].color = color;
        vertexes_[vertex_index_ + 2].color = color;
        vertexes_[vertex_index_ + 3].color = color;
        faces_[face_index_].index[0] = vertex_index_;
        faces_[face_index_].index[1] = vertex_index_ + 1;
        faces_[face_index_].index[2] = vertex_index_ + 2;
        faces_[face_index_ + 1].index[0] = vertex_index_ + 2;
        faces_[face_index_ + 1].index[1] = vertex_index_ + 3;
        faces_[face_index_ + 1].index[2] = vertex_index_;
        face_index_ += 2;
        vertex_index_ += 4;
    }

    vbo_ = new VertexBufferGPU( vertexes_, vertex_index_, faces_, face_index_ );
    delete[] vertexes_;
    delete[] faces_;
}

void SpriteFont::SetText( const Vector2& position, const std::string& str, const Vector4& color )
{
    f32 x = position.x;
    f32 y = position.y;
    text_length_ = str.size();
    u32 vertex_index_ = 0;
    Vertex* vertexes_ = vbo_->Lock();

    for ( u32 i = 0; i < text_length_; i++ ) {
        u8 ch = str[i];

        if ( ch == '\n' ) {
            y -= font_height_;
            x = position.x;
            continue;
        }

        if ( ch < 32 || ch > 128 ) {
            ch = '?';
        }

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad( cdata_,
                            512,    // width of font canvas
                            512,    // height of font canvas
                            ch - 32, // position of character in font
                            &x,     // current position
                            &y,
                            &q,     // resulted quad
                            1 );     // 1 tex coords for opengl (0 for d3d)
        // note: x,y position is advanced by font character size
        //f32 w = q.x1-q.x0;
        f32 h = q.y1 - q.y0;
        q.y0 = y - h;
        q.y1 = y;
        //VERTEX 0
        vertexes_[vertex_index_].pos   = Vector3( q.x0, q.y0, 0.0 );
        vertexes_[vertex_index_].tex   = Vector2( q.s0, q.t1 );
        vertexes_[vertex_index_].color = color;
        //VERTEX 1
        vertexes_[vertex_index_ + 1].pos   = Vector3( q.x1, q.y0, 0.0 );
        vertexes_[vertex_index_ + 1].tex   = Vector2( q.s1, q.t1 );
        vertexes_[vertex_index_ + 1].color = color;
        //VERTEX 2
        vertexes_[vertex_index_ + 2].pos   = Vector3( q.x1, q.y1, 0.0 );
        vertexes_[vertex_index_ + 2].tex   = Vector2( q.s1, q.t0 );
        vertexes_[vertex_index_ + 2].color = color;
        //VERTEX 3
        vertexes_[vertex_index_ + 3].pos   = Vector3( q.x0, q.y1, 0.0 );
        vertexes_[vertex_index_ + 3].tex   = Vector2( q.s0, q.t0 );
        vertexes_[vertex_index_ + 3].color = color;
        vertex_index_ += 4;
    }

    vbo_->Unlock();
}

void SpriteFont::Draw( const AttributeBinding& binding )
{
    vbo_->Draw( binding, 0, text_length_ * 2 );
}

SpriteFont::~SpriteFont()
{
    delete[] cdata_;
    delete vbo_;
    delete texture_;
}

}//namespace opengl
}//namespace base
*/
