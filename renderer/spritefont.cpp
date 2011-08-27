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
//nclude <fstream>

namespace base {
namespace opengl {


SpriteFont::SpriteFont(std::string filename, f32 height):m_height(height),m_valid(false)
{

   fread(ttf_buffer, 1, 1<<20, fopen(filename.c_str(), "rb"));      
         
	cdata = new stbtt_bakedchar[96];
   stbtt_BakeFontBitmap((const unsigned char*)ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, (stbtt_bakedchar*)cdata); // no guarantee this fits!
   // can free ttf_buffer at this point
   glGenTextures(1, &ftex);
   glBindTexture(GL_TEXTURE_2D, ftex);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 512,512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, temp_bitmap);
   // can free temp_bitmap at this point
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
   
   _vertexes	= NULL;
   _faces		= NULL;
   textVBO		= NULL; 
   
}

void SpriteFont::setText (f32 x, f32 y, char *text)
{
	if (textVBO!=NULL)
		delete textVBO;
	// assume orthographic projection with units = screen pixels, origin at top left
   
   int length = strlen(text);
   vertex_index = 0;
   face_index = 0;
   //allocating memory for vertex data 
   _vertexes =  new Vertex[length*4];
   _faces = 	new Face[length*2];
   
   while (length) {
      if (*text >= 32 && *text < 128) {
         stbtt_aligned_quad q;
         stbtt_GetBakedQuad((stbtt_bakedchar*)cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl,0=old d3d
         
          //VERTEX 0
 		_vertexes[vertex_index].pos.x = q.x0+x;
 		_vertexes[vertex_index].pos.y = q.y0+y;
 		_vertexes[vertex_index].pos.z = 0.0;
 		_vertexes[vertex_index].n.x = 0.0;
 		_vertexes[vertex_index].n.y = 0.0;
 		_vertexes[vertex_index].n.z = 1.0;
 		_vertexes[vertex_index].tex.x = q.s0;
 		_vertexes[vertex_index].tex.y = q.t1;
 		//VERTEX 1
 		_vertexes[vertex_index+1].pos.x = q.x1+x;
 		_vertexes[vertex_index+1].pos.y = q.y0+y;
 		_vertexes[vertex_index+1].pos.z = 0.0;
 		_vertexes[vertex_index+1].n.x = 0.0;
 		_vertexes[vertex_index+1].n.y = 0.0;
 		_vertexes[vertex_index+1].n.z = 1.0;
 		_vertexes[vertex_index+1].tex.x = q.s1;
 		_vertexes[vertex_index+1].tex.y = q.t1;
 		//VERTEX 2
 		_vertexes[vertex_index+2].pos.x = q.x1+x;
 		_vertexes[vertex_index+2].pos.y = q.y1+y;
 		_vertexes[vertex_index+2].pos.z = 0.0;
 		_vertexes[vertex_index+2].n.x = 0.0;
 		_vertexes[vertex_index+2].n.y = 0.0;
 		_vertexes[vertex_index+2].n.z = 1.0;
 		_vertexes[vertex_index+2].tex.x = q.s1;
 		_vertexes[vertex_index+2].tex.y = q.t0;
 		//VERTEX 3
 		_vertexes[vertex_index+3].pos.x = q.x0+x;
 		_vertexes[vertex_index+3].pos.y = q.y1+y;
 		_vertexes[vertex_index+3].pos.z = 0.0;
 		_vertexes[vertex_index+3].n.x = 0.0;
 		_vertexes[vertex_index+3].n.y = 0.0;
 		_vertexes[vertex_index+3].n.z = 1.0;
 		_vertexes[vertex_index+3].tex.x = q.s0;
 		_vertexes[vertex_index+3].tex.y = q.t0;
 		//indexes
 		 _faces[face_index].index[0] = vertex_index;
 		 _faces[face_index].index[1] = vertex_index+1;
 		 _faces[face_index].index[2] = vertex_index+2;

 		 _faces[face_index+1].index[0] = vertex_index+2;
 		 _faces[face_index+1].index[1] = vertex_index+3;
 		 _faces[face_index+1].index[2] = vertex_index;

 		 _faces[face_index].n.x = 0.0;
 		 _faces[face_index].n.y = 0.0;
 		 _faces[face_index].n.z = 1.0;

 		 _faces[face_index+1].n.x = 0.0;
 		 _faces[face_index+1].n.y = 0.0;
 		 _faces[face_index+1].n.z = 1.0;

 		 face_index += 2;

      }

      ++text;
      --length;
      vertex_index+=4;
   }
   
	textVBO = new VertexBuffer(_vertexes, vertex_index, _faces, face_index);   
}

void SpriteFont::Draw (const AttributeBinding& binding)
{
	glBindTexture(GL_TEXTURE_2D, ftex);
	if (textVBO!=NULL)
		textVBO->Draw(binding);
}

SpriteFont::~SpriteFont()
{
	//delete cdata;
	delete textVBO;
}
		
}//namespace opengl
}//namespace base