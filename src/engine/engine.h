#pragma once

#include "base/types.h"

namespace base {

namespace opengl { class DeviceContext; struct Renderer; }

class Engine {
public:
    ~Engine();

    static void init(opengl::DeviceContext& context);
    static void shutdown();
    NEGINE_API static opengl::DeviceContext& context();
    NEGINE_API static opengl::Renderer& renderer();
private:
    Engine(opengl::DeviceContext& context);

    opengl::Renderer* renderer_;
    opengl::DeviceContext& GL;
    static Engine* instance_;
};

} // namespace base