#include "game/meshcomponent.h"
#include "game/gameobject.h"
#include "game/transformcomponent.h"

MeshComponent::MeshComponent()
    : transform_(nullptr)
{
}

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

void MeshComponent::onDetach()
{
    transform_ = nullptr;
}