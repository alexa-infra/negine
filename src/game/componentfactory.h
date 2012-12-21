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
    virtual ComponentType type() const = 0;
    virtual Component* create() const = 0;
};

template<typename T>
class GenericComponentFactory : public ComponentFactory
{
public:
    ComponentType type() const {
        return (ComponentType)T::typeId;
    }
    Component* create() const {
        return new T;
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
        if (map_contains<ComponentFactoryMap>(factories_, (ComponentType)T::typeId))
            return;
        ComponentFactory* factory = new GenericComponentFactory<T>;
        factories_[(ComponentType)T::typeId] = factory;
    }
    Component* create(ComponentType type) const;
};

class ComponentRegistry : public base::Singleton<ComponentFactoryRegistry>
{
public:
    template<typename T>
    static void Register()
    {
        if (!hasInstance()) init();
        instance()->Register<T>();
    }
    static Component* create(ComponentType type)
    {
        return instance()->create(type);
    }
private:
    DISALLOW_COPY_AND_ASSIGN(ComponentRegistry);
};

template<typename T>
struct ComponentRegistryHelper
{
    ComponentRegistryHelper()
    {
        ComponentRegistry::Register<T>();
    }
};

#define REGISTER_COMPONENT(Type) namespace {    \
        ComponentRegistryHelper<Type> register_##Type;    \
    }

