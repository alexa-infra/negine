#pragma once

#include <map>
#include <list>
#include "base/types.h"
#include "game/componenttypes.h"

class Component;
typedef std::list<Component*> ComponentList;
typedef std::map<ComponentType, ComponentList> ComponentMap;

class Object;
typedef std::map<std::string, Object*> ObjectMap;

class Scene
{
public:
	ComponentMap components_;
	ObjectMap objects_;

	~Scene();

	Object* spawnObject(const std::string& name);

	Object* getObject(const std::string& name);

	void destroyObject(const std::string& name);

	void destroyObject(Object* obj);

private:
	void clear();

	void destroyObjectR(Object* obj);

	void addComponent(Component* component);

	void removeComponents(Object* obj);

	friend class Object;
};

/*

Scene sc;

Object* root = sc.getObject("root");

Object* obj = sc.spawnObject("name");
obj.addComponent(camera);
obj.addComponent(sprite);
obj.setParent(root);












*/