/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "app/sdlapp.h"

#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include "base/math/matrix.h"
#include "renderer/vertexbuffer.h"
#include "renderer/md3mesh.h"
#include "renderer/particlesystem.h"
#include "base/timer.h"

class SDLDemo : public SDLApp {
protected:
    base::opengl::Texture* texture_;
    base::opengl::Program* program_;
    base::math::Matrix4 projection_;
    base::math::Matrix4 modelview_;
    base::opengl::VertexBuffer* buffer_;
    std::vector<base::opengl::VertexBuffer*> mesh_;
    base::opengl::ParticleSystem* ps_;
	base::Timer timer_;

public:
    SDLDemo(u32 width, u32 height);
    ~SDLDemo();

protected:
    void OnFrame();
    void OnMotion(f32 dx, f32 dy);
};