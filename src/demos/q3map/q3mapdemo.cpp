/**
 * \file
 * \brief       q3 map demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "renderer/q3map.h"
#include "renderer/camera.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include "renderer/vertexbuffer.h"
#include <assert.h>

using namespace base;
using namespace base::math;
using namespace base::opengl;

class Demo : public Application {
protected:
    Program* program_;
    q3maploader* q3map_;
    Camera camera_;
    TextureLoader texure_loader_;
    Matrix4 projection_;
    Matrix4 cameraTransform_;
    u32 keypressed_;
    struct Cube;
    std::vector<Cube> cubes_;
public:
    Demo() {
        program_ = NULL;
        buffer_ = NULL;
        q3map_ = NULL;
        keypressed_ = 0;
        //camera_.set_position(Vector3(0.f, 0.f, 500.f));
        //camera_.set_position(Vector3(221.4f, 5.0f, 63.0f));
        //camera_.set_position(Vector3(200, 100, -1200));
        //camera_.set_position(Vector3(0, 0, 0));
        camera_.set_position(Vector3(28.4461f, 41.9335f, 300.19f));

        camera_.set_pitch(0);
        camera_.set_head(-math::pi);
            
        camera_.set_aspect(width_ / (f32)height_);
        camera_.set_fov(45.f);
        camera_.set_zNear(10);
        camera_.set_zFar(5000);
        camera_.Update();

        cameraTransform_ = camera_.GetModelView(); 
        projection_ = camera_.GetProjection();

        base::FileBinary fb("maps/q3dm6.bsp");
        q3map_ = new q3maploader(fb);
        q3map_->load();
        q3map_->PreloadTextures( texure_loader_ );
        std::vector<Vertex> vv;
        std::vector<Face> ff;
        Cube c = AddCube(vv, ff, camera_.position());
        //cubes_.push_back(c);
        buffer_ = new VertexBufferGPU(&vv[0], vv.size(), &ff[0], ff.size());
        
        program_ = LoadProgram("q3map.shader");
    }
    virtual ~Demo() {

        delete program_;
        delete q3map_;
        delete buffer_;
    }
protected:
    bool InterFrustum(const Cube& c) const {
        for (int i=0; i<6; i++) {
            const Plane& p = camera_.planes()[i];
            u8 r = p.BoxOnPlaneSide(c.min, c.max);
            if (r == 2)
                return false;
        }
        return true;
    }
    void OnFrame() {
        UpdateWorld();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        program_->Bind();
        program_->set_uniform(base::opengl::UniformVars::Projection, projection_);
        program_->set_uniform(base::opengl::UniformVars::Modelview, cameraTransform_);

        Texture* t = texure_loader_.Load( "checker.png" );
        program_->set_uniform(base::opengl::UniformVars::Diffuse, t);

        for (int i=0; i<cubes_.size(); i++)
        {
            const Cube& c = cubes_[i];
            if (!InterFrustum(c))
            {
                std::cout << "culled" << std::endl;
                continue;
            }
            else
            {
                std::cout << "not culled" << std::endl;
            }
            buffer_->Draw(program_->binding(), c.face_start, 12); 
        }
        q3map_->render(camera_, program_, texure_loader_);
        program_->Unbind();

        assert(glGetError() == GL_NO_ERROR);
        Application::OnFrame();
    }
    VertexBuffer* buffer_;
    void SetVertexPos(Vertex* v, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3) {
        v[0].pos = v0;
        v[1].pos = v1;
        v[2].pos = v2;
        v[3].pos = v3;
    }
    Face SetFace(i16 i1, i16 i2, i16 i3)
    {
        Face f;
        f.index[0] = i1;
        f.index[1] = i2;
        f.index[2] = i3;
        return f;
    }
    void AddCubeSide(std::vector<Vertex>& vert, std::vector<Face>& face, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& n)
    {
        Vertex v[4];
        SetVertexPos(v, v0, v1, v2, v3);
        SetVertexUV(v);
        SetVertexN(v, n);

        i16 i = static_cast<i16>(vert.size());
        face.push_back(SetFace(i+0, i+1, i+2));
        face.push_back(SetFace(i+2, i+3, i+0));
        for(int j=0;j<4; j++)
            vert.push_back(v[j]);
    }
    struct Cube
    {
        Vector3 max, min;
        i32 face_start;
    };
    Cube AddCube(std::vector<Vertex>& v, std::vector<Face>& f, const math::Vector3& position)
    {
        i32 s = 10;
        Vector3 v0 = Vector3(0, 0, 0) + position;
        Vector3 v1 = Vector3(0, 0, s) + position;
        Vector3 v2 = Vector3(s, 0, s) + position;
        Vector3 v3 = Vector3(s, 0, 0) + position;
        Vector3 v4 = Vector3(0, s, 0) + position;
        Vector3 v5 = Vector3(0, s, s) + position;
        Vector3 v6 = Vector3(s, s, s) + position;
        Vector3 v7 = Vector3(s, s, 0) + position;

        Cube cube;
        cube.face_start = f.size();
        cube.max = v6;
        cube.min = v0;

        AddCubeSide(v, f, v0, v1, v2, v3, Vector3(0, -1, 0));
        AddCubeSide(v, f, v1, v5, v6, v2, Vector3(0, 0, 1));
        AddCubeSide(v, f, v5, v4, v7, v6, Vector3(0, 0, 1));
        AddCubeSide(v, f, v4, v0, v3, v7, Vector3(0, 0, -1));
        AddCubeSide(v, f, v0, v4, v5, v1, Vector3(-1, 0, 0));
        AddCubeSide(v, f, v2, v6, v7, v3, Vector3(1, 0, 0));
        return cube;
    }
    void SetVertexN(Vertex* v, const math::Vector3& n)
    {
        for( int i=0; i<4; i++ ) v[i].n = n;
    }
    void SetVertexUV(Vertex* v)
    {
        v[0].tex = math::Vector2(0.0f, 0.0f);
        v[1].tex = math::Vector2(1.0f, 0.0f);
        v[2].tex = math::Vector2(1.0f, 1.0f);
        v[3].tex = math::Vector2(0.0f, 1.0f);
    }

    void OnMotion(i32 x, i32 y, i32 dx, i32 dy) {
/*
        camera_.set_head( camera_.head()  + deg_to_rad * dx);

        if (fabs (camera_.pitch() + deg_to_rad * dy ) < base::math::pi/2.0f )
        {
            camera_.set_pitch(camera_.pitch() + deg_to_rad * dy );
        }
        else
        {
            if (dy != 0 )
                camera_.set_pitch( base::math::pi/2.0f * dy / fabs( dy ) );
        }*/
    }

    void OnKeyboardDown(u8 key) {

        if (key == 'w') {
            keypressed_ |= 1;
        } else if (key == 's') {
            keypressed_ |= 2;
        } else if (key == 'a') {
            keypressed_ |= 4;
        } else if (key == 'd') {
            keypressed_ |= 8;
        } else if ( key == 'j' ) {
            keypressed_ |= 16;
        } else if ( key == 'k' ) {
            keypressed_ |= 32;
        } else if ( key == 'h' ) {
            keypressed_ |= 64;
        } else if ( key == 'l' ) {
            keypressed_ |= 128;
        } else if ( key == 'p' ) {
            std::cout << "camera pos: " << camera_.position() << '\n';
            std::cout << "camera pitch: " << camera_.pitch() << '\n';
            std::cout << "camera head: " << camera_.head() << '\n';
            std::cout.flush();
        } else if ( key == 'o' ) {
            for ( int i = 0; i < 6; i++ )
                std::cout << camera_.planes()[i].Normal() << ' ' << camera_.planes()[i].D() << '\n';
            std::cout.flush();
        } else if ( key == 'm' ) {
            std::cout << camera_.GetClipMatrix() << std::endl;
        }
    }
    void OnKeyboardUp(u8 key) {

        if (key == 'w') {
            keypressed_ ^= 1;
        } else if (key == 's') {
            keypressed_ ^= 2;
        } else if (key == 'a') {
            keypressed_ ^= 4;
        } else if (key == 'd') {
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

    void UpdateWorld() {
        const f32 speed = 1.0f;
        if ( keypressed_ & 1 )
            camera_.set_position( camera_.position() + camera_.forward() * speed );
        if ( keypressed_ & 2 )
            camera_.set_position( camera_.position() - camera_.forward() * speed );
        if ( keypressed_ & 4 )
            camera_.set_position( camera_.position() + camera_.right()   * speed );
        if ( keypressed_ & 8 )
            camera_.set_position( camera_.position() - camera_.right()   * speed );
        if ( keypressed_ & 16 )
            camera_.set_pitch( camera_.pitch() + deg_to_rad );
        if ( keypressed_ & 32 )
            camera_.set_pitch( camera_.pitch() - deg_to_rad );
        if ( keypressed_ & 64 )
            camera_.set_head( camera_.head() + deg_to_rad );
        if ( keypressed_ & 128 )
            camera_.set_head( camera_.head() - deg_to_rad );
        
        camera_.Update();
        cameraTransform_ = camera_.GetModelView();
    }
};

int main() {
    Demo app;
    app.Run();

    return 0;
}
