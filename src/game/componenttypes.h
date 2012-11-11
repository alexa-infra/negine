#pragma once

namespace ComponentTypes
{
	enum ComponentType
	{
		Camera = 0,
		Mesh,
		Transform,
		Player,
		ParticleSystem,
		Billboard,
		Animation,

		Count
	};
}
typedef ComponentTypes::ComponentType ComponentType;

