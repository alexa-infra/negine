/**
 * \file
 * \brief       Scene demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "renderer/camera.h"
#include "renderer/glprogram.h"
#include <assert.h>
#include "game/scene.h"
#include "game/gameobject.h"
#include "game/cameracomponent.h"
#include "game/transformcomponent.h"
#include "game/meshcomponent.h"
#include "game/componentfactory.h"

#include <sstream>

using namespace base;
using namespace base::math;
using namespace base::opengl;

template<typename T>
std::string stringify(const T& val)
{
    std::stringstream ss;
    ss << val;
    return ss.str();
}

class Demo : public Application
{
	Program        program_;
	Scene* scene_;
public:
	Demo() {
		scene_ = new Scene;

		GameObject* o1 = scene_->spawnObject("root");
		GameObject* o2 = scene_->spawnObject("player");
        GameObject* o3 = scene_->spawnObject("node");
		GameObject* grid = scene_->spawnObject("cubegrid");
		o2->setParent(o1);
        o3->setParent(o1);
		grid->setParent(o3);

        CubeMesh* mesh = new CubeMesh;
        for (i32 i=-5; i<5; i++) for (i32 j=-5; j<5; j++) for (i32 k=-5; k<5; k++)
        {
            std::stringstream ss;
            ss << "cube" << i << "_" << j << "_" << k;
            std::string name = ss.str();
            ///name = name + stringify(i) + "_" + stringify(j) + "_" + stringify(k);
            GameObject* cube = scene_->spawnObject(name);
            cube->addComponentT<MeshComponent>()->mesh_ = mesh;
            cube->setParent(grid);
            cube->transform_->as<TransformComponent>()
                ->local() *= Matrix4::Translation( Vector3( 12.0f * i, 12.0f * j, 12.0f * k ) );
        }

        CameraComponent* camera = o2->addComponentT<CameraComponent>();
        TransformComponent* cameraTransf = o2->transform_->as<TransformComponent>();

		//MeshComponent* mesh = grid->addComponentT<MeshComponent>();
		TransformComponent* cubeTransf = o3->transform_->as<TransformComponent>();
		
		//o4->addComponentT<MeshComponent>();
		//TransformComponent* subcubeTransf = o4->transform_->as<TransformComponent>();
		//subcubeTransf->local() *= Matrix4::Translation( Vector3( 0, 20, 0 ) );

		cubeTransf->local() *= Matrix4::Translation( Vector3( 0, 0, -800 ) );

		cameraTransf->local() *= Matrix4::Translation( Vector3( 0, 0, 0 ) );

		camera->camera_.set_pitch( 0 );
		camera->camera_.set_head( 0 );
		camera->camera_.set_aspect( width_ / ( f32 )height_ );
		camera->camera_.set_fov( 45.0f );
		camera->camera_.set_zNear( 1 );
		camera->camera_.set_zFar( 1000 );
		
		const char vertexSh[] = {
"attribute vec3 position;"
"attribute vec4 col;"

"uniform mat4 projection_matrix;"
"uniform mat4 modelview_matrix;"

"varying vec4 vcolor;"

"void main(void) {"
"    vcolor = col;"
"    gl_Position = projection_matrix * modelview_matrix * vec4(position, 1.0);"
"}"
		};
		const char fragmentSh[] = {
"varying vec4 vcolor;"

"void main(void) {"
"    gl_FragColor = vcolor;"
"}"
		};

		std::string status;
		bool result = program_.CreateFromText(vertexSh, fragmentSh, status);
		assert(result);
        assert( glGetError() == GL_NO_ERROR );
	}
	virtual ~Demo() {
		delete scene_;
		program_.Destroy();

		ComponentRegistry::shutdown();
	}
protected:
	void OnFrame() {
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
		glEnable( GL_DEPTH_TEST );
		program_.Bind();

		ComponentList& cameras = scene_->components_[ComponentTypes::Camera];
		ComponentList& meshes = scene_->components_[ComponentTypes::Mesh];

        TransformComponent* grid = scene_->getObject("cubegrid")->transform_->as<TransformComponent>();
        grid->local() *= Matrix4::RotationY( 30 / 60.f * deg_to_rad );
		CameraComponent* cameraComponent = cameras.front()->as<CameraComponent>();
		cameraComponent->update();
		Camera& camera = cameraComponent->camera_;

		program_.set_uniform( base::opengl::UniformVars::Projection, camera.GetProjection() );

        u32 pos = 1;
		for(ComponentList::iterator it = meshes.begin();
			it != meshes.end();
			++it)
		{
			Component* component = *it;
			MeshComponent* mesh = component->as<MeshComponent>();

			//mesh->transform_->local() *= Matrix4::RotationX( 30 / 60.f * deg_to_rad * 500 /(f32)pos );

			program_.set_uniform( base::opengl::UniformVars::Modelview,
				camera.GetModelView() * mesh->transform_->world( ) );

			mesh->mesh_->draw();

            pos+=1;
		}

		//
		program_.Unbind();
		assert( glGetError() == GL_NO_ERROR );
		Application::OnFrame();
	}

	void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
	}

	void OnKeyboardDown( u8 key ) {
	}

};

int main()
{
	Demo app;
	app.Run();
	return 0;
}
