#include "game/meshcomponent.h"
#include "game/gameobject.h"
#include "game/transformcomponent.h"

void MeshComponent::onAttach()
{
	Component* comp = object_->findNearest(ComponentTypes::Transform);
	if (comp == nullptr)
	{
		transform_ = nullptr;
		return;
	}
	transform_ = comp->as<TransformComponent>();
}