#pragma once

#include "game/component.h"
#include "renderer/cubemesh.h"

class TransformComponent;

class MeshComponent : public Component
{
public:
	static ComponentType type() { return ComponentTypes::Mesh; }
	
	base::opengl::CubeMesh mesh_;

	TransformComponent* transform_;

	void onAttach();

	void onDetach()
	{
	}
};