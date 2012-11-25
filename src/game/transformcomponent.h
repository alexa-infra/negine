#pragma once

#include <list>
#include "game/component.h"
#include "game/componentfactory.h"
#include "base/math/matrix.h"

class TransformComponent;
typedef std::list<TransformComponent*> TransformComponentList;

class TransformComponent : public ComponentBase<ComponentTypes::Transform>
{
public:
	TransformComponent();

	void onAttach();
	void onDetach();

    const base::math::Matrix4& world();
    const base::math::Matrix4& local() const { return local_; }
    base::math::Matrix4& local() { 
        dirty_ = true;
        return local_;
    }

    bool isDirty() const
    {
        if (dirty_) return true;
        if (parentTransform_ != nullptr) return parentTransform_->isDirty();
        return false;
    }
private:
    TransformComponent* parentTransform_;
    bool dirty_;
    base::math::Matrix4 world_;
    base::math::Matrix4 local_;
};
REGISTER_COMPONENT(TransformComponent);