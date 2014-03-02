#pragma once

#include "base/types.h"
#include "base/singleton.h"

namespace base {

namespace opengl { struct Renderer; }
namespace phys { class Physics; }
namespace game { class Scene; }

class Engine : public Singleton<Engine> {
public:
    Engine();
    ~Engine();

    NEGINE_API static opengl::Renderer& renderer();
    NEGINE_API static phys::Physics& physics();
    NEGINE_API static game::Scene& scene();
private:
    opengl::Renderer* renderer_;
    phys::Physics* physics_;
    game::Scene* scene_;
};

} // namespace base
