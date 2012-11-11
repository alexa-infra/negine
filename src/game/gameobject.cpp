#include "game/gameobject.h"
#include "game/componentfactory.h"
#include "game/scene.h"
#include "game/components.h"

Object::Object(const std::string& name, Scene& scene) 
	: scene_(scene)
	, name_(name)
	, parent_(nullptr)
{
}

void Object::setParent(Object* parent)
{
	if (parent_ != nullptr)
		parent_->removeChild(this);
	if (parent != nullptr)
		parent->children_.push_back(this);
	parent_ = parent;
	reAttachComponentsR();
}

bool Object::hasComponent(ComponentType type) const
{
	return map_contains<ComponentArray>(components_, type);
}

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

void Object::removeChild(Object* obj)
{
	for(ObjectList::iterator it = children_.begin();
		it != children_.end();
		++it) {
		Object* child = *it;
		if (child == obj) {
			children_.erase(it);
			return;
		}
	}
}

void Object::reAttachComponentsR()
{
	reAttachComponents();
	for(ObjectList::iterator it = children_.begin();
		it != children_.end();
		++it) {
		Object* child = *it;
		child->reAttachComponentsR();
	}
}

void Object::reAttachComponents()
{
	for(ComponentArray::iterator it=components_.begin();
		it != components_.end();
		++it)
	{
		Component* component = it->second;
		component->onAttach();
	}
}