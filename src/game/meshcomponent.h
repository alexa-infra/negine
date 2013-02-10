#pragma once

#include "game/component.h"
#include "game/componentfactory.h"
#include "render/cubemesh.h"

class TransformComponent;

class MeshComponent : public ComponentBase<ComponentTypes::Mesh>
{
public:
    base::opengl::CubeMesh* mesh_;
    TransformComponent* transform_;

    MeshComponent();

    void onAttach();

    void onDetach();
};
REGISTER_COMPONENT(MeshComponent);
