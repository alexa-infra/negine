#pragma once

#include "base/types.h"

namespace base
{
namespace opengl
{

#ifdef OS_WIN
    #define _APIENTRY __stdcall
#else
    #define _APIENTRY
#endif
#define _APIENTRYP _APIENTRY*

typedef u32 GLenum;
typedef u8 GLboolean;
typedef u32 GLbitfield;
typedef i8 GLbyte;
typedef i8 GLshort;
typedef i32 GLint;
typedef i32 GLsizei;
typedef u8 GLubyte;
typedef u16 GLushort;
typedef u32 GLuint;
typedef u16 GLhalf;
typedef f32 GLfloat;
typedef f32 GLclampf;
typedef f64 GLdouble;
typedef f64 GLclampd;
typedef void GLvoid;

typedef char GLchar;
typedef uptr GLintptr;
typedef uptr GLsizeiptr;
typedef i64 GLint64;
typedef u64 GLuint64; 

#define GL_FALSE                          0
#define GL_TRUE                           1

#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505

#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_DOUBLE                         0x140A

#define GL_LINES                          0x0001
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006 

#define GL_STENCIL_INDEX                  0x1901
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908

#define GL_RG                             0x8227

/* StringName */
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
/* TextureMagFilter */
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
/* TextureMinFilter */
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803 

#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MIRRORED_REPEAT                0x8370
#define GL_REPEAT                         0x2901 

#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_RECTANGLE              0x84F5 
#define GL_TEXTURE0                       0x84C0 

#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_UNIFORM_BUFFER                 0x8A11 

#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA 

#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA 

#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87

#define GL_SAMPLER_2D                     0x8B5E

#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH_WRITEMASK                0x0B72

#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_BLEND                          0x0BE2

#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305 

#define GL_FLOAT_MAT4                     0x8B5C
#define GL_FLOAT_VEC4                     0x8B52
#define GL_SAMPLER_2D                     0x8B5E

typedef void (_APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture); 
typedef void (_APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader); 
typedef void (_APIENTRYP PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar *name); 
typedef void (_APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (_APIENTRYP PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer); 
typedef void (_APIENTRYP PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size); 
typedef void (_APIENTRYP PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (_APIENTRYP PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (_APIENTRYP PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures); 
typedef void (_APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (_APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (_APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays); 
typedef void (_APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (_APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void (_APIENTRYP PFNGLGETBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
typedef GLvoid* (_APIENTRYP PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (_APIENTRYP PFNGLUNMAPBUFFERPROC) (GLenum target); 
typedef void (_APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (_APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (_APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
typedef void (_APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (_APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (_APIENTRYP PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader); 
typedef void (_APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers); 
typedef void (_APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (_APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index); 
typedef void (_APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices); 
typedef void (_APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers); 
typedef void (_APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target); 
typedef void (_APIENTRYP PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name); 
typedef void (_APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog); 
typedef void (_APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog); 
typedef void (_APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params); 
typedef void (_APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params); 
typedef GLint (_APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name); 
typedef void (_APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program); 
typedef void (_APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
typedef void (_APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program); 
typedef void (_APIENTRYP PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels); 
typedef void (_APIENTRYP PFNGLTEXPARAMETERIPROC) (GLenum target, GLenum pname, GLint param); 
typedef void (_APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0); 
typedef void (_APIENTRYP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2); 
typedef void (_APIENTRYP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); 
typedef void (_APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
typedef void (_APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer); 

typedef void (_APIENTRYP PFNGLCLEARPROC) (GLbitfield mask);
typedef void (_APIENTRYP PFNGLCLEARCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (_APIENTRYP PFNENABLEPROC) (GLenum cap);
typedef GLenum (_APIENTRYP PFNGLGETERRORPROC) (void);
typedef void (_APIENTRYP PFNGLDISABLEPROC) (GLenum cap);
typedef const GLubyte* (_APIENTRYP PFNGETSTRINGPROC) (GLenum name);
typedef void (_APIENTRYP PFNBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
typedef void (_APIENTRYP PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);

}
}
