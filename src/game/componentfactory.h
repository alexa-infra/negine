#pragma once

#include <assert.h>
#include "base/types.h"
#include "base/singleton.h"
#include "base/stdext.h"
#include "game/componenttypes.h"

class Component;

class ComponentFactory
{
public:
	virtual ComponentType type() = 0;
	virtual Component* create() = 0;
};

template<typename T>
class GenericComponentFactory : public ComponentFactory
{
public:
	ComponentType type() {
		return T::type();
	}
	Component* create() {
		T* t = new T;
		t->type_ = T::type();
		return t;
	}
};
typedef std::map<ComponentType, ComponentFactory*> ComponentFactoryMap;

class ComponentFactoryRegistry
{
	ComponentFactoryMap factories_;
public:
	ComponentFactoryRegistry();
	~ComponentFactoryRegistry();

	template<typename T>
	void Register()
	{
		GenericComponentFactory<T>* factory =
			new GenericComponentFactory<T>;
		assert( map_contains<ComponentFactoryMap>(factories_, factory->type()) == false );
		factories_[factory->type()] = factory;
	}

	Component* create(ComponentType type) const;
};

class ComponentRegistry : base::Singleton<ComponentFactoryRegistry>
{
public:
	template<typename T>
	static void Register()
	{
		instance()->Register<T>();
	}
	static Component* create(ComponentType type)
	{
		return instance()->create(type);
	}
private:
	DISALLOW_COPY_AND_ASSIGN(ComponentRegistry);
};