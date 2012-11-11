#pragma once

#include "game/componenttypes.h"
#include <map>
#include <list>

class GameObject;

class Component
{
public:
	Component();

	GameObject* object_;
	ComponentType type_;

	template<typename T>
	T *const as()
	{
		return dynamic_cast<T *const>(this);
	}

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
};
typedef std::map<ComponentType, Component*> ComponentArray;
typedef std::list<Component*> ComponentList;
typedef std::map<ComponentType, ComponentList> ComponentMap;

