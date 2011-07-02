#pragma once 

#include "app/glutapp.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include "base/math/matrix.h"
#include "renderer/vertexbuffer.h"
#include "renderer/spritegroup.h"
#include "renderer/md3mesh.h"

class GlutSampleWindow : public GlutWindow {
private:
    base::opengl::Texture* texture_;
    base::opengl::Program* program_;
    base::math::Matrix4 projection_;
    base::math::Matrix4 modelview_;
    base::opengl::VertexBuffer* buffer_;
    base::opengl::SpriteGroup* sg_;
    std::vector<base::opengl::Mesh*> mesh_;
public:
    GlutSampleWindow(i32 width = 640, i32 height = 480); 
    virtual ~GlutSampleWindow();
protected:
    virtual void OnDisplay(void);
    virtual void OnReshape(i32 width, i32 height);
    virtual void OnMotion(i32 x, i32 y);
};

