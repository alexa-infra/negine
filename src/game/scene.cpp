#include "game/scene.h"
#include "game/componentfactory.h"

Component* Object::addComponent(ComponentType type)
{
	assert(components_[type] == nullptr);
	Component* comp = ComponentRegistry::create(type);
	comp->object_ = this;
	components_[type] = comp;
	scene_.addComponent(comp);
	reAttachComponentsR();
	return comp;
}