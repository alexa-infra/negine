#pragma once

#include "base/types.h"
#include "base/singleton.h"

namespace base {

namespace opengl { struct Renderer; }
namespace phys { class Physics; }

class Engine : public Singleton<Engine> {
public:
    Engine();
    ~Engine();

    NEGINE_API static opengl::Renderer& renderer();
    NEGINE_API static phys::Physics& physics();
private:
    opengl::Renderer* renderer_;
    phys::Physics* physics_;
};

} // namespace base
