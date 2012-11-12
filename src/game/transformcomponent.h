#pragma once

#include <list>
#include "game/component.h"
#include "base/math/matrix.h"

class TransformComponent;
typedef std::list<TransformComponent*> TransformComponentList;

class TransformComponent : public Component
{
public:
	static ComponentType type() { return ComponentTypes::Transform; }

	TransformComponent();

	void onAttach();
	void onDetach();

	void updateR();

	TransformComponent* parentTransform_;
	TransformComponentList dependent_;
	bool dirty_;
	base::math::Matrix4 world_;
	base::math::Matrix4 local_;

	const base::math::Matrix4& world();

	void removeDependent(TransformComponent* comp);
};