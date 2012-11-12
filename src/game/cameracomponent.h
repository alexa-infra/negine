#pragma once

#include "game/component.h"
#include "renderer/camera.h"

class TransformComponent;

class CameraComponent : public Component
{
public:
	static ComponentType type() { return ComponentTypes::Camera; }
	
	base::opengl::Camera camera_;

	CameraComponent();

	TransformComponent* transform_;

	void onAttach();

	void onDetach()
	{
	}

	void update();
};