#pragma once

#include "game/component.h"
#include "game/componentfactory.h"
#include "renderer/camera.h"

class TransformComponent;

class CameraComponent : public ComponentBase<ComponentTypes::Camera>
{
public:
	base::opengl::Camera camera_;

	CameraComponent();

	TransformComponent* transform_;

	void onAttach();

	void onDetach();

	void update();
};
REGISTER_COMPONENT(CameraComponent);