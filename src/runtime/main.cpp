#include "app/app.h"
#include <cmath>
#include <boost/python.hpp>
#include <string>
#include "render/material.h"
#include "engine/meshbuilder.h"
#include "render/model.h"
#include "render/gpuprogram.h"
#include "base/timer.h"
#include "base/py.h"
#include "math/matrix-inl.h"
#include "engine/resourceref.h"
#include "render/renderstate.h"

#include "render/texture.h"

#include "game/components/transform.h"
#include "game/components/camera.h"
#include "base/log.h"

using namespace base;
using namespace boost::python;

extern "C" NEGINE_EXPORT void initnegine_runtime(void);

static const char vertexShader[] = 
    "#version 150\n"
    "\n"
    "in vec3 position;\n"
    "in vec3 normal;\n"
    "\n"
    "uniform mat4 mvp;\n"
    "\n"
    "out vec3 n;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    n = 0.5*normalize(normal) + 0.5;\n"
    "    vec4 pos = vec4(position, 1);\n"
    "    gl_Position = mvp * pos;\n"
    "}\n"
    "";
static const char pixelShader[] = 
    "#version 150\n"
    "\n"
    "in vec3 n;\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "    fragColor = vec4(n, 1);\n"
    "}\n"
    "";

static const char vertexShader1[] = 
    "#version 150\n"
    "\n"
    "in vec3 position;\n"
    "in vec2 uv;\n"
    "\n"
    "out vec2 tex;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    tex = uv;\n"
    "    vec4 pos = vec4(position, 1);\n"
    "    gl_Position = pos;\n"
    "}\n"
    "";
static const char pixelShader1[] = 
    "#version 150\n"
    "\n"
    "uniform sampler2D atexture;\n"
    "in vec2 tex;\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "    fragColor = texture(atexture, tex);\n"
    "}\n"
    "";

static Application* intstance_ = nullptr;

class GameCamera {
public:
    game::Transform transform;
    game::Camera camera;
    game::Entity object;

    GameCamera() {
        object.add(&transform);
        object.add(&camera);
    }
};

class Demo : public Application
{
    base::opengl::Renderer ren;
    base::opengl::GpuProgram prog;
    base::opengl::GpuProgram prog2;

    GameCamera cam;
    base::opengl::Mesh mesh;
    u32 keypressed_; 
    base::imp::MeshBuilder bb;
    base::Timer timer_;
    opengl::Model* umesh;

    opengl::Framebuffer fbo;

public:
    Demo(const std::string& filename) : ren(GL), prog(GL), prog2(GL), fbo(GL) {
        intstance_ = this;
        Py_Initialize();
        PyImport_AppendInittab("negine_core", initnegine_core);
        PyImport_AppendInittab("negine_runtime", initnegine_runtime);
        object m = import("__main__");
        global = object(m.attr("__dict__"));

        exec_file(filename.c_str(), global, global);

        keypressed_ = 0; 

        bb.beginSurface();
        bb.addVertex(math::vec3f( 1, -1, -1), math::vec2f(0, 0));
        bb.addVertex(math::vec3f( 1,  1, -1), math::vec2f(0, 1));
        bb.addVertex(math::vec3f(-1,  1, -1), math::vec2f(1, 1));
        bb.addVertex(math::vec3f(-1, -1, -1), math::vec2f(1, 0));

        bb.addPolygon(0, 1, 2);
        bb.addPolygon(0, 2, 3);
        bb.endSurface();

        ren.init();
        {
            //bb.readOBJ("trunk.obj");
            //bb.readOBJ("leaves.obj");

            {
                bb.getDrawingList(mesh);
            }
        }

        prog.setAttribute("position", base::opengl::VertexAttrs::tagPosition);
        prog.setAttribute("normal", base::opengl::VertexAttrs::tagNormal);

        prog.setShaderSource(base::opengl::ShaderTypes::VERTEX, vertexShader);
        prog.setShaderSource(base::opengl::ShaderTypes::PIXEL, pixelShader);
        prog.complete();

        prog2.setAttribute("position", opengl::VertexAttrs::tagPosition);
        prog2.setAttribute("uv", opengl::VertexAttrs::tagTexture);
        prog2.setShaderSource(base::opengl::ShaderTypes::VERTEX, vertexShader1);
        prog2.setShaderSource(base::opengl::ShaderTypes::PIXEL, pixelShader1);
        prog2.complete();

        cam.transform.setPosition( base::math::vec3f( 0.f, 0.f, -5.f ) );
        cam.transform.setPitch( 50 * base::math::deg_to_rad );
        cam.transform.setHead( 180 * base::math::deg_to_rad );
        cam.camera.setPerspective(width_ / ( f32 )height_, 45.0f, 1, 1000);
        
        ResourceRef checkers("checkers");
        checkers.loadDefault<opengl::Texture>("checker.png");
        ASSERT(checkers.resourceAs<opengl::Texture>() != nullptr);
        
        //fbo.addTarget(opengl::InternalTypes::RGBA8);
        fbo.addTargetTexture(checkers.resourceAs<opengl::Texture>());
        fbo.addTarget(opengl::InternalTypes::D32F);
        fbo.resizeWindow(math::vec2i(width_, height_));
        //fbo.complete();
        
        GL_ASSERT(GL);

        ResourceRef model("themodel");
        model.loadDefault<opengl::Model>("trunk.obj");
        umesh = model.resourceAs<opengl::Model>();
    }
    virtual ~Demo() {
    }
protected:
    base::Params para;
    void OnFrame() {
        UpdateWorld();

        //ren.rendering();

        GL.setFramebuffer(&fbo);

        GL.setDepthWrite(true);
        GL.setDepthTest(true);
        GL.setViewport(math::vec4f(0u, 0u, width_, height_));
        GL.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GL.setProgram(&prog);
        para["mvp"] = cam.camera.clipMatrix();
        prog.setParams(para);
        
        size_t meshCount = umesh->surfaceCount();
        for(size_t i=0; i<meshCount; i++) {
            const opengl::Mesh& m = umesh->surfaceAt(i).mesh;
            GL.renderState().render(m, 0, m.numIndexes());
        }

        
        GL.setFramebuffer(nullptr);
        GL.setDepthWrite(false);
        GL.setDepthTest(false);
        GL.setViewport(math::vec4f(0u, 0u, width_, height_));
        GL.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        base::Params pp;
        ResourceRef checkers("checkers");
        pp["atexture"] = checkers.resourceAs<opengl::Texture>();
        GL.setProgram(&prog2);
        prog2.setParams(pp);
        GL.renderState().render(mesh, 0, mesh.numIndexes());

        GL_ASSERT(GL);
        SDLApp::OnFrame();
    }

    void OnKeyboardDown( u8 key ) {
        if ( key == 'w' ) {
            keypressed_ |= 1;
        } else if ( key == 's' ) {
            keypressed_ |= 2;
        } else if ( key == 'a' ) {
            keypressed_ |= 4;
        } else if ( key == 'd' ) {
            keypressed_ |= 8;
        } else if ( key == 'j' ) {
            keypressed_ |= 16;
        } else if ( key == 'k' ) {
            keypressed_ |= 32;
        } else if ( key == 'h' ) {
            keypressed_ |= 64;
        } else if ( key == 'l' ) {
            keypressed_ |= 128;
        }
    }
    void OnKeyboardUp( u8 key ) {
        if ( key == 'w' ) {
            keypressed_ ^= 1;
        } else if ( key == 's' ) {
            keypressed_ ^= 2;
        } else if ( key == 'a' ) {
            keypressed_ ^= 4;
        } else if ( key == 'd' ) {
            keypressed_ ^= 8;
        } else if ( key == 'j' ) {
            keypressed_ ^= 16;
        } else if ( key == 'k' ) {
            keypressed_ ^= 32;
        } else if ( key == 'h' ) {
            keypressed_ ^= 64;
        } else if ( key == 'l' ) {
            keypressed_ ^= 128;
        }
    }

    void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
        cam.transform.turnHead( math::deg_to_rad * dx);

        if (fabs (cam.transform.pitch() + math::deg_to_rad * dy ) < base::math::pi/2.0f ) {
            cam.transform.turnPitch( math::deg_to_rad * dy );
        } else if (dy != 0 ) {
            cam.transform.setPitch( base::math::pi/2.0f * dy / fabs( (f32)dy ) );
        }
    }

    void UpdateWorld() {
        const f32 speed = 0.1f;

        if ( keypressed_ & 1 ) {
            cam.transform.moveForward( speed );
        }

        if ( keypressed_ & 2 ) {
            cam.transform.moveBackward( speed );
        }

        if ( keypressed_ & 4 ) {
            cam.transform.moveLeft( speed );
        }

        if ( keypressed_ & 8 ) {
            cam.transform.moveRight( speed );
        }

        if ( keypressed_ & 16 ) {
            if ( fabs( cam.transform.pitch() + math::deg_to_rad ) < math::pi / 2.0f )
                cam.transform.turnPitch( math::deg_to_rad );
        }

        if ( keypressed_ & 32 ) {
            if ( fabs( cam.transform.pitch() - math::deg_to_rad ) < math::pi / 2.0f )
                cam.transform.turnPitch( -math::deg_to_rad );
        }

        if ( keypressed_ & 64 ) {
            cam.transform.turnHead( math::deg_to_rad );
        }

        if ( keypressed_ & 128 ) {
            cam.transform.turnHead( -math::deg_to_rad );
        }

        game::Transform::updateTree(&cam.object);
        game::Camera::updateTree(&cam.object);
    } 

    object global;
};

int main(int argc, char* argv[])
{
    Demo app(argv[1]);
    app.Run();
    return 0;
}

static Application& globalApp() {
    ASSERT(intstance_ != nullptr);
    return *intstance_;
}


BOOST_PYTHON_MODULE(negine_runtime)
{
    def("globalApp", globalApp, return_value_policy<reference_existing_object>());
    class_<Demo, boost::noncopyable>("App", no_init)
        .add_property( "context", make_function( static_cast< opengl::DeviceContext& (Application::*)() >( &Application::context ), return_value_policy<reference_existing_object>() ) );
}