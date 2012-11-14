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

using namespace base;
using namespace base::math;
using namespace base::opengl;

class Demo : public Application
{
	Program        program_;
	Scene* scene_;
public:
	Demo() {
		ComponentRegistry::init();

		scene_ = new Scene;

		GameObject* o1 = scene_->spawnObject("root");
		GameObject* o2 = scene_->spawnObject("player");
		GameObject* o3 = scene_->spawnObject("cube");
		GameObject* o4 = scene_->spawnObject("subcube");
		o2->setParent(o1);
		o3->setParent(o1);
		o4->setParent(o3);
		CameraComponent* camera = o2->addComponentT<CameraComponent>();
		TransformComponent* cameraTransf = o2->addComponentT<TransformComponent>();

		MeshComponent* mesh = o3->addComponentT<MeshComponent>();
		TransformComponent* cubeTransf = o3->addComponentT<TransformComponent>();
		
		o4->addComponentT<MeshComponent>();
		TransformComponent* subcubeTransf = o4->addComponentT<TransformComponent>();
		subcubeTransf->local_ *= Matrix4::Translation( Vector3( 0, 20, 0 ) );
		subcubeTransf->updateR();

		cubeTransf->local_ *= Matrix4::Translation( Vector3( 0, 0, -40 ) );
		cubeTransf->updateR();

		cameraTransf->local_ *= Matrix4::Translation( Vector3( 0, 0, 0 ) );
		cameraTransf->updateR();

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

		CameraComponent* cameraComponent = cameras.front()->as<CameraComponent>();
		cameraComponent->update();
		Camera& camera = cameraComponent->camera_;

		program_.set_uniform( base::opengl::UniformVars::Projection, camera.GetProjection() );

		for(ComponentList::iterator it = meshes.begin();
			it != meshes.end();
			++it)
		{
			Component* component = *it;
			MeshComponent* mesh = component->as<MeshComponent>();

			mesh->transform_->local_ *= Matrix4::RotationX( 30 / 60.f * deg_to_rad );
			mesh->transform_->updateR();

			program_.set_uniform( base::opengl::UniformVars::Modelview,
				camera.GetModelView() * mesh->transform_->world( ) );

			mesh->mesh_.draw(program_.binding());
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
