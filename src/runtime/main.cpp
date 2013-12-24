#include "app/app.h"
#include <cmath>
#include <boost/python.hpp>
#include <string>
#include "render/material.h"
#include "render/meshbuilder.h"
#include "render/mesh.h"
#include "render/gpuprogram.h"
#include "base/timer.h"
#include "base/py.h"
#include "render/model_loader.h"
#include "math/matrix-inl.h"

#include "game/components/transform.h"
#include "game/components/camera.h"

using namespace base;
using namespace boost::python;

extern "C" NEGINE_EXPORT void initnegine_runtime(void);

static const char vertexShader[] = 
    "#version 150\n"
    "\n"
    "attribute vec3 position;\n"
    "attribute vec2 uv;\n"
    "\n"
    "uniform mat4 mvp;\n"
    "\n"
    "varying vec2 texCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    texCoord = uv;\n"
    "    vec4 pos = vec4(position, 1);\n"
    "    gl_Position = mvp * pos;\n"
    "}\n"
    "";
static const char pixelShader[] = 
    "uniform float time;\n"
    "uniform vec4 viewport;\n"
    "varying vec2 texCoord;\n"
    "\n"
    "void main() {\n"
    "    vec4 newColor = vec4(1.0, 0, 0, 1.0);\n"
    "    float x = texCoord.s / viewport.x;\n"
    "    float y = texCoord.t / viewport.y;\n"
    "    newColor.g = sin(x * cos(time/15.0) * 120.0) +\n"
    "                cos(y * sin(time/10.0) * 120.0) +\n"
    "                sin(sqrt(y*y + x*x) * 40.0);\n"
    "    gl_FragColor = newColor;\n"
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
    GameCamera cam;
    base::opengl::Mesh mesh;
    u32 keypressed_; 
    base::imp::MeshBuilder bb;
    base::Timer timer_;

public:
    Demo(const std::string& filename) : ren(GL), prog(GL) {
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
        prog.setAttribute("uv", base::opengl::VertexAttrs::tagTexture);

        prog.setShaderSource(base::opengl::ShaderTypes::VERTEX, vertexShader);
        prog.setShaderSource(base::opengl::ShaderTypes::PIXEL, pixelShader);
        prog.complete();

        cam.transform.setPosition( base::math::vec3f( 0.f, 0.f, -5.f ) );
        cam.transform.setPitch( 50 * base::math::deg_to_rad );
        cam.transform.setHead( 180 * base::math::deg_to_rad );
        cam.camera.setPerspective(width_ / ( f32 )height_, 45.0f, 1, 1000);

        opengl::ModelLoader::load("trunk.obj");
    }
    virtual ~Demo() {
    }
protected:
    base::Params para;
    void OnFrame() {
        UpdateWorld();

        ren.rendering();

        GL.renderState().program.set(&prog);
        para["mvp"] = math::Matrix4::Identity();
        para["time"] = timer_.elapsed() / 1000.0f;
        para["viewport"] = math::vec4f(1u, 1u, 0u, 0u);
        prog.setParams(para);
        for (u32 i=0; i<bb.surfaces.size(); i++) {
            imp::Surface& s = bb.surfaces[i];
            GL.renderState().render(mesh, s.polygon, s.polygonCount * 3);
        }

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