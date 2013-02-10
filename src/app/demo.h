/**
 * \file
 * \brief       Demonstration class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "app/app.h"
#include "render/gltexture.h"
#include "render/glprogram.h"
#include "base/math/matrix.h"
#include "render/vertexbuffer.h"
#include "render/md3mesh.h"
#include "render/spritefont.h"
#include "render/particlesystem.h"
#include "render/particlesystemrenderer.h"
#include "render/camera.h"
#include "base/timer.h"
#include "render/md5mesh.h"
#include "render/md5anim.h"
#include "render/md5renderer.h"
#include "render/wirebox.h"

#include "render/md3renderer.h"
#include "render/q3map.h"

using base::math::Matrix4;
using base::math::Vector2;
using base::math::Vector3;
using base::math::Vector4;
using base::opengl::Texture;
using base::opengl::Program;

using base::opengl::SpriteFont;
using base::resource::ParticleSystem;
using base::opengl::ParticleSystemRenderer;
using base::opengl::Camera;
using base::opengl::TextureLoader;
using base::opengl::AttributeBinding;

using base::resource::Md3Model;
using base::opengl::Md3Renderer;

using base::resource::ParticleSystemSetting;
using base::resource::Entity;
using base::opengl::Md5Renderer;
using base::Timer;
using base::resource::Md5Anim;
using base::opengl::q3maploader;
using base::opengl::WireBox;

class Demo : public Application
{
private:
    Texture* texture_;
    Texture* texture_bump_;
    Program* program_;
    Program* program_hud_;
    Program* program_font_;
    Program* program_wirebox_;
    Matrix4 projection_;
    Matrix4 modelview_;
    SpriteFont* font_;
    ParticleSystem* ps_;
    ParticleSystemRenderer* ps_renderer_;
    Md5Renderer* md5_renderer_;
    WireBox* wire_box_;

    Md3Renderer* md3_renderer_;

    Matrix4 cameraTransform_;
    Matrix4 modelTransform_;
    Timer timer_;

    Vector2 cursor_;

    f32 frame_diff_;

    Camera camera_;
    WireBox* camera_wirebox_;
    q3maploader* q3map_;

    TextureLoader texure_loader_;

    Entity*  entity;
public:
    Demo( i32 width = 640, i32 height = 480 );
    virtual ~Demo();
protected:
    void OnFrame( void );
    void OnReshape( i32 width, i32 height );
    void OnMotion( i32 x, i32 y, i32 dx, i32 dy );
    void OnKeyboard( unsigned char key, int x, int y );

private:
    Program* LoadProgram( const std::string& filename );
};

