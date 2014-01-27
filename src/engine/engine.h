#pragma once

#include "base/types.h"

namespace base {

namespace opengl { class DeviceContext; struct Renderer; }
namespace phys { class Physics; }

class Engine {
public:
    ~Engine();

    static void init(opengl::DeviceContext& context);
    static void shutdown();
    NEGINE_API static opengl::DeviceContext& context();
    NEGINE_API static opengl::Renderer& renderer();
    NEGINE_API static phys::Physics& physics();
private:
    Engine(opengl::DeviceContext& context);

    opengl::Renderer* renderer_;
    opengl::DeviceContext& GL;
    phys::Physics* physics_;

    static Engine* instance_;
};

} // namespace base