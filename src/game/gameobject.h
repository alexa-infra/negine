#pragma once

#include <deque>
#include <map>
#include "game/componenttypes.h"

class Scene;

class Component;
typedef std::map<ComponentType, Component*> ComponentArray;

class Object;
typedef std::deque<Object*> ObjectList;

class Object
{
public:
	Object(const std::string& name, Scene& scene);

	ComponentArray components_;
	Object* parent_;
	ObjectList children_;
	Scene& scene_;
	std::string name_;

	void setParent(Object* parent);

	bool hasComponent(ComponentType type) const;

	Component* addComponent(ComponentType type);
private:
	void removeChild(Object* obj);

	void reAttachComponentsR();

	void reAttachComponents();
};
typedef std::map<std::string, Object*> ObjectMap;