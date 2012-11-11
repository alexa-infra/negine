#include "game/componentfactory.h"
#include "game/cameracomponent.h"
#include "game/animationcomponent.h"
#include "game/component.h"

Component* ComponentFactoryRegistry::create(ComponentType type) const
{
	ComponentFactory* factory = nullptr;
	bool result = try_find<ComponentFactoryMap>(factories_, type, factory);
	assert( result == true );
	return factory->create();
}

ComponentFactoryRegistry::ComponentFactoryRegistry()
{
	Register<CameraComponent>();
	Register<AnimationComponent>();
}

ComponentFactoryRegistry::~ComponentFactoryRegistry()
{
	for(ComponentFactoryMap::iterator it = factories_.begin();
		it != factories_.end();
		++it)
	{
		ComponentFactory* factory = it->second;
		delete factory;
	}
	factories_.clear();
}