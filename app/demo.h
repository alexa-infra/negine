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

using namespace base::math;
using namespace base::opengl;

class Demo : public Application {
private:
    Texture* texture_;
    Program* program_;
    Program* program_hud_;
    Program* program_font_;
    Matrix4 projection_;
    Matrix4 modelview_;
    VertexBuffer* buffer_;
    std::vector<VertexBuffer*> mesh_;
    SpriteFont *font_;
    ParticleSystem* ps_;
    Texture* texture_ps_;

    Matrix4 cameraTransform_;
    Matrix4 modelTransform_;
    base::Timer timer_;

    Vector2 cursor_;
    std::string title_text_;
public:
    Demo(i32 width = 640, i32 height = 480); 
    virtual ~Demo();
protected:
    void OnFrame(void);
    void OnReshape(i32 width, i32 height);
    void OnMotion(i32 x, i32 y, i32 dx, i32 dy);
    void OnKeyboard(unsigned char key, int x, int y);

private:
    Program* LoadProgram(const std::string& filename);
};

