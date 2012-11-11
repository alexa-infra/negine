#include "game/scene.h"
#include <assert.h>
#include "base/stdext.h"
#include "base/types.h"
#include "game/components.h"
#include "game/gameobject.h"

Scene::~Scene()
{
	clear();
}

Object* Scene::spawnObject(const std::string& name)
{
	assert( map_contains<ObjectMap>(objects_, name) == false );
	Object* obj = new Object(name, *this);
	objects_[name] = obj;
	return obj;
}

Object* Scene::getObject(const std::string& name) {
	Object* obj;
	bool result = try_find<ObjectMap>(objects_, name, obj);
	assert( result == true );
	return obj;
}

void Scene::destroyObject(const std::string& name)
{
	Object* obj = getObject(name);
	destroyObjectR(obj);
}

void Scene::destroyObject(Object* obj)
{
	destroyObjectR(obj);
}

void Scene::clear()
{
	while(!objects_.empty())
	{
		ObjectMap::iterator it = objects_.begin();
		Object* obj = it->second;
		destroyObjectR(obj);
	}
}

void Scene::destroyObjectR(Object* obj)
{
	obj->setParent(nullptr);
	objects_.erase(obj->name_);
	removeComponents(obj);
	for(ObjectList::iterator it = obj->children_.begin();
		it != obj->children_.end();
		++it) {
		Object* child = *it;
		destroyObjectR(child);
	}
	delete obj;
}

void Scene::addComponent(Component* component)
{
	components_[component->type_].push_back(component);
}

void Scene::removeComponents(Object* obj)
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
