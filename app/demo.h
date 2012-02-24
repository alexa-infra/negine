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
#include "renderer/md3mesh.h"
#include "renderer/spritefont.h"
#include "renderer/particlesystem.h"
#include "base/timer.h"

class Demo : public Application {
private:
    base::opengl::Texture* texture_;
    base::opengl::Program* program_;
    base::opengl::Program* program_hud_;
    base::math::Matrix4 projection_;
    base::math::Matrix4 modelview_;
    base::opengl::VertexBuffer* buffer_;
    std::vector<base::opengl::VertexBuffer*> mesh_;
    base::opengl::SpriteFont *font_;
    base::opengl::ParticleSystem* ps_;
    base::opengl::Texture* texture_ps_;

    base::math::Matrix4 cameraTransform_;
    base::math::Matrix4 modelTransform_;
    base::Timer timer_;

    base::math::Vector2 cursor_;
public:
    Demo(i32 width = 640, i32 height = 480); 
    virtual ~Demo();
protected:
    void OnFrame(void);
    void OnReshape(i32 width, i32 height);
    void OnMotion(i32 x, i32 y, i32 dx, i32 dy);
    void OnKeyboard(unsigned char key, int x, int y);
};

