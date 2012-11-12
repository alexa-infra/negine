#include "game/cameracomponent.h"
#include "game/gameobject.h"
#include "game/transformcomponent.h"
#include <assert.h>

CameraComponent::CameraComponent()
	: transform_(nullptr)
{
}

void CameraComponent::onAttach()
{
	Component* comp = object_->findNearest(ComponentTypes::Transform);
	if (comp == nullptr)
	{
		transform_ = nullptr;
		return;
	}
	transform_ = comp->as<TransformComponent>();
}

void CameraComponent::update()
{
	assert(transform_ == nullptr);
	base::math::Vector4 pos =
		transform_->world() * base::math::Vector3(0.0f, 0.0f, 0.0f);
	camera_.set_position(pos.xyz());
	camera_.Update();
}