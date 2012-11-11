#pragma once

#include <map>
#include <list>
#include "base/types.h"
#include "game/componenttypes.h"

class Component;
typedef std::list<Component*> ComponentList;
typedef std::map<ComponentType, ComponentList> ComponentMap;

class GameObject;
typedef std::map<std::string, GameObject*> ObjectMap;

class Scene
{
public:
	ComponentMap components_;
	ObjectMap objects_;

	~Scene();

	GameObject* spawnObject(const std::string& name);

	GameObject* getObject(const std::string& name);

	void destroyObject(const std::string& name);

	void destroyObject(GameObject* obj);

private:
	void clear();

	void destroyObjectR(GameObject* obj);

	void addComponent(Component* component);

	void removeComponents(GameObject* obj);

	friend class GameObject;
};

/*

Scene sc;

GameObject* root = sc.getObject("root");

GameObject* obj = sc.spawnObject("name");
obj.addComponent(camera);
obj.addComponent(sprite);
obj.setParent(root);












*/