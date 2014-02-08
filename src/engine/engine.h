#pragma once

#include "base/types.h"
#include "base/singleton.h"

namespace base {

namespace opengl { class DeviceContext; struct Renderer; }
namespace phys { class Physics; }

class Engine : public Singleton<Engine> {
public:
    Engine(opengl::DeviceContext& context);
    ~Engine();

    NEGINE_API static opengl::DeviceContext& context();
    NEGINE_API static opengl::Renderer& renderer();
    NEGINE_API static phys::Physics& physics();
private:
    opengl::Renderer* renderer_;
    opengl::DeviceContext& GL;
    phys::Physics* physics_;
};

} // namespace base