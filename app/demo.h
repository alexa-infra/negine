/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once 

#include "app/app.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include "base/math/matrix.h"
#include "renderer/vertexbuffer.h"
#include "renderer/spritegroup.h"
#include "renderer/md3mesh.h"
#include "renderer/spritefont.h"
#include "base/timer.h"

class Demo : public Application {
private:
    base::opengl::Texture* texture_;
    base::opengl::Program* program_;
    base::math::Matrix4 projection_;
    base::math::Matrix4 modelview_;
    base::opengl::VertexBuffer* buffer_;
    base::opengl::SpriteGroup* sg_;
    std::vector<base::opengl::VertexBuffer*> mesh_;
    base::opengl::SpriteFont *font;

    base::math::Matrix4 cameraTransform_;
    base::math::Matrix4 modelTransform_;
    base::Timer timer_;
public:
    Demo(i32 width = 640, i32 height = 480); 
    virtual ~Demo();
protected:
    virtual void OnFrame(void);
    virtual void OnReshape(i32 width, i32 height);
    virtual void OnMotion(i32 x, i32 y);
    virtual void OnKeyboard(unsigned char key, int x, int y);
};

