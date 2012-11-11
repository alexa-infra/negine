#pragma once

#include "game/components.h"
#include <deque>
#include <assert.h>
#include "base/stdext.h"
#include "base/types.h"

class Scene;
class Object;
typedef std::deque<Object*> ObjectList;

class Object
{
public:
	Object(const std::string& name, Scene& scene) 
		: scene_(scene)
		, name_(name)
		, parent_(nullptr)
	{
	}

	ComponentArray components_;
	Object* parent_;
	ObjectList children_;
	Scene& scene_;
	std::string name_;

	void setParent(Object* parent)
	{
		if (parent_ != nullptr)
			parent_->removeChild(this);
		if (parent != nullptr)
			parent->children_.push_back(this);
		parent_ = parent;
		reAttachComponentsR();
	}

	bool hasComponent(ComponentType type) const
	{
		return map_contains<ComponentArray>(components_, type);
	}

	Component* addComponent(ComponentType type);
private:
	void removeChild(Object* obj)
	{
		for(ObjectList::iterator it = children_.begin();
			it != children_.end();
			++it) {
			Object* child = *it;
			if (child == obj) {
				children_.erase(it);
				return;
			}
		}
	}

	void reAttachComponentsR()
	{
		reAttachComponents();
		for(ObjectList::iterator it = children_.begin();
			it != children_.end();
			++it) {
			Object* child = *it;
			child->reAttachComponentsR();
		}
	}

	void reAttachComponents()
	{
		for(ComponentArray::iterator it=components_.begin();
			it != components_.end();
			++it)
		{
			Component* component = it->second;
			component->onAttach();
		}
	}
};
typedef std::map<std::string, Object*> ObjectMap;

class Scene
{
public:
	ComponentMap components_;
	ObjectMap objects_;

	~Scene()
	{
		clear();
	}

	Object* spawnObject(const std::string& name)
	{
		assert( map_contains<ObjectMap>(objects_, name) == false );
		Object* obj = new Object(name, *this);
		objects_[name] = obj;
		return obj;
	}

	Object* getObject(const std::string& name) {
		Object* obj;
		bool result = try_find<ObjectMap>(objects_, name, obj);
		assert( result == true );
		return obj;
	}

	void destroyObject(const std::string& name)
	{
		Object* obj = getObject(name);
		destroyObjectR(obj);
	}

	void destroyObject(Object* obj)
	{
		destroyObjectR(obj);
	}

private:
	void clear()
	{
		while(!objects_.empty())
		{
			ObjectMap::iterator it = objects_.begin();
			Object* obj = it->second;
			destroyObjectR(obj);
		}
	}

	void destroyObjectR(Object* obj)
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

	void addComponent(Component* component)
	{
		components_[component->type_].push_back(component);
	}

	void removeComponents(Object* obj)
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