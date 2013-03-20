#include "game/scene.h"
#include "base/stdext.h"
#include "base/types.h"
#include "game/component.h"
#include "game/gameobject.h"
#include "base/debug.h"

Scene::~Scene()
{
    clear();
}

GameObject* Scene::spawnObject(const std::string& name, bool hasTransform)
{
    ASSERT( map_contains<ObjectMap>(objects_, name) == false );
    GameObject* obj = new GameObject(name, *this);
    objects_[name] = obj;
    if (hasTransform)
        obj->addComponent(ComponentTypes::Transform);
    return obj;
}

GameObject* Scene::getObject(const std::string& name) {
    GameObject* obj;
    bool result = try_find<ObjectMap>(objects_, name, obj);
    ASSERT( result == true );
    return obj;
}

void Scene::destroyObject(const std::string& name)
{
    GameObject* obj = getObject(name);
    destroyObjectR(obj);
}

void Scene::destroyObject(GameObject* obj)
{
    destroyObjectR(obj);
}

void Scene::clear()
{
    while(!objects_.empty())
    {
        ObjectMap::iterator it = objects_.begin();
        GameObject* obj = it->second;
        destroyObjectR(obj);
    }
}

void Scene::destroyObjectR(GameObject* obj)
{
    obj->setParent(nullptr);
    objects_.erase(obj->name_);
    removeComponents(obj);
    while(!obj->children_.empty()) {
        ObjectList::iterator it = obj->children_.begin();
        GameObject* child = *it;
        destroyObjectR(child);
    }
    delete obj;
}

void Scene::addComponent(Component* component)
{
    components_[component->type()].push_back(component);
}

void Scene::removeComponents(GameObject* obj)
{
    for(ComponentMap::iterator it=components_.begin();
        it != components_.end(); ++it) {
        ComponentList& componentList = it->second;
        for(ComponentList::iterator itr = componentList.begin();
            itr != componentList.end();
            ) {
            Component* component = *itr;
            if (component->object_ == obj) {
                itr = componentList.erase(itr);
                continue;
            }
            ++itr;
        }
    }
}
