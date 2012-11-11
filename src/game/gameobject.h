#pragma once

#include <deque>
#include <map>
#include "game/componenttypes.h"

class Scene;

class Component;
typedef std::map<ComponentType, Component*> ComponentArray;

class GameObject;
typedef std::deque<GameObject*> ObjectList;

class GameObject
{
public:
	GameObject(const std::string& name, Scene& scene);

	ComponentArray components_;
	GameObject* parent_;
	ObjectList children_;
	Scene& scene_;
	std::string name_;

	void setParent(GameObject* parent);

	bool hasComponent(ComponentType type) const;

	Component* addComponent(ComponentType type);
private:
	void removeChild(GameObject* obj);

	void reAttachComponentsR();

	void reAttachComponents();
};
typedef std::map<std::string, GameObject*> ObjectMap;