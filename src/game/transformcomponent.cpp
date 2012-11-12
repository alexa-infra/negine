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
	dependent_.clear();
}

void TransformComponent::onDetach()
{
	if (parentTransform_ == nullptr)
		return;
	parentTransform_->removeDependent(this);
}

void TransformComponent::removeDependent(TransformComponent* comp)
{
	for(TransformComponentList::iterator it = dependent_.begin();
		it != dependent_.end();
		++it)
	{
		TransformComponent* dep = *it;
		if (dep == comp)
		{
			dependent_.erase(it);
			break;
		}
	}
}

void TransformComponent::updateR()
{
	dirty_ = true;
	for(TransformComponentList::iterator it = dependent_.begin();
		it != dependent_.end();
		++it)
	{
		TransformComponent* dep = *it;
		dep->updateR();
	}
}

const base::math::Matrix4& TransformComponent::world()
{
	if (!dirty_)
		return world_;

	if (parentTransform_ != nullptr)
		world_ = parentTransform_->world() * local_;
	else
		world_ = local_;

	dirty_ = false;
	return world_;
}