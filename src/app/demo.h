/**
 * \file
 * \brief       Demonstration class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
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
#include "renderer/camera.h"
#include "base/timer.h"

using base::math::Matrix4;
using base::math::Vector2;
using base::math::Vector3;
using base::math::Vector4;
using base::opengl::Texture;
using base::opengl::Program;
using base::opengl::VertexBuffer;
using base::opengl::VertexBufferGPU;
using base::opengl::SpriteFont;
using base::opengl::ParticleSystem;
using base::opengl::Camera;
using base::opengl::TextureLoader;
using base::opengl::AttributeBinding;
using base::opengl::load_md3_se;
using base::opengl::Mesh;
using base::opengl::ParticleSystemSetting;
using base::Timer;

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
    Timer timer_;

    Vector2 cursor_;
    std::string title_text_;

    f32 frame_diff_;

    Camera camera_;

    TextureLoader texure_loader_;
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

