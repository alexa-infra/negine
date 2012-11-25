#include "game/transformcomponent.h"
#include "game/gameobject.h"

TransformComponent::TransformComponent()
{
	local_ = base::math::Matrix4::Identity();
}

void TransformComponent::onAttach()
{
	if (object_ == nullptr)
		return;
	dirty_ = true;
	Component* comp = object_->findParent(ComponentTypes::Transform);
	if (comp == nullptr)
		parentTransform_ = nullptr;
	parentTransform_ = comp->as<TransformComponent>();
}

void TransformComponent::onDetach()
{
}

const base::math::Matrix4& TransformComponent::world()
{
    if (!isDirty())
		return world_;

	if (parentTransform_ != nullptr)
		world_ = parentTransform_->world() * local_;
	else
		world_ = local_;

	dirty_ = false;
	return world_;
}