#include "game/gameobject.h"
#include "game/componentfactory.h"
#include "game/scene.h"
#include "game/component.h"

GameObject::GameObject(const std::string& name, Scene& scene) 
    : scene_(scene)
    , name_(name)
    , parent_(nullptr)
{
}

void GameObject::setParent(GameObject* parent)
{
    if (parent_ != nullptr)
        parent_->removeChild(this);
    if (parent != nullptr)
        parent->children_.push_back(this);
    parent_ = parent;
    reAttachComponentsR();
}

bool GameObject::hasComponent(ComponentType type) const
{
    return map_contains<ComponentArray>(components_, type);
}

Component* GameObject::addComponent(ComponentType type)
{
    assert(components_[type] == nullptr);
    Component* comp = ComponentRegistry::create(type);
    comp->object_ = this;
    components_[type] = comp;
    scene_.addComponent(comp);
    reAttachComponentsR();
    
    if (type == ComponentTypes::Transform)
        transform_ = comp;
    
    return comp;
}

void GameObject::removeChild(GameObject* obj)
{
    for(ObjectList::iterator it = children_.begin();
        it != children_.end();
        ++it) {
        GameObject* child = *it;
        if (child == obj) {
            children_.erase(it);
            return;
        }
    }
}

void GameObject::reAttachComponentsR()
{
    reAttachComponents();
    for(ObjectList::iterator it = children_.begin();
        it != children_.end();
        ++it) {
        GameObject* child = *it;
        child->reAttachComponentsR();
    }
}

void GameObject::reAttachComponents()
{
    for(ComponentArray::iterator it=components_.begin();
        it != components_.end();
        ++it) {
        Component* component = it->second;
        component->onAttach();
    }
}

Component* GameObject::findParent(ComponentType type) const
{
    return find(type, parent_);
}

Component* GameObject::findSibling(ComponentType type) const
{
    Component* component = nullptr;
    if (!try_find<ComponentArray>(components_, type, component))
        return nullptr;
    return component;
}

Component* GameObject::findNearest(ComponentType type) const
{
    return find(type, this);
}

Component* GameObject::find(ComponentType type, const GameObject* obj)
{
    while(obj != nullptr)
    {
        Component* component = nullptr;
        if (try_find<ComponentArray>(obj->components_, type, component))
            return component;
        obj = obj->parent_;
    }
    return nullptr;
}