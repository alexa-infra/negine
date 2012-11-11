#pragma once

#include "game/component.h"

class CameraComponent : public Component
{
public:
	static ComponentType type() { return ComponentTypes::Camera; }

	void onAttach()
	{
	}

	void onDetach()
	{
	}
};