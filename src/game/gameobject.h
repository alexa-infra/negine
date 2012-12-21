#pragma once

#include <deque>
#include <map>
#include <string>
#include "game/componenttypes.h"

class Scene;

class Component;
typedef std::map<ComponentType, Component*> ComponentArray;

class GameObject;
typedef std::deque<GameObject*> ObjectList;

class TransformComponent;

class GameObject
{
public:
    GameObject(const std::string& name, Scene& scene);

    ComponentArray components_;
    GameObject* parent_;
    ObjectList children_;
    Scene& scene_;
    std::string name_;
    Component* transform_;

    void setParent(GameObject* parent);

    bool hasComponent(ComponentType type) const;

    Component* addComponent(ComponentType type);

    template<typename T>
    T* addComponentT()
    {
        Component* component = addComponent((ComponentType)T::typeId);
        return dynamic_cast<T*>(component);
    }

    Component* findParent(ComponentType type) const;

    Component* findSibling(ComponentType type) const;

    Component* findNearest(ComponentType type) const;
private:
    static Component* find(ComponentType type, const GameObject* obj);

    void removeChild(GameObject* obj);

    void reAttachComponentsR();

    void reAttachComponents();
};
typedef std::map<std::string, GameObject*> ObjectMap;
