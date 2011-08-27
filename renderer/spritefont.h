/**
 * @author  Victor Sukochev <sukochevvv@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
 
#pragma once

#include "renderer/vertexbuffer.h"
#include "renderer/sprite.h"
#include <stdio.h>

#include <string>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace base {
namespace opengl {

	class SpriteFont 
	{
	public:
		//Sprite *sprites;

		SpriteFont(std::string filename, f32 height);
		~SpriteFont();

		void setText (f32 x, f32 y, char *text); 
		void Draw(const AttributeBinding& binding);

	private:
		f32 m_height;
		bool m_valid;

		char ttf_buffer[1<<20];
		unsigned char temp_bitmap[512*512];

		void* cdata; // ASCII 32..126 is 95 glyphs
		GLuint ftex;

		Vertex *_vertexes;
		Face *_faces;

		int vertex_index;
	    int face_index;

	    VertexBuffer *textVBO;
	};

}//namespace opengl
}//namespace base


