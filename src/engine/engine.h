#pragma once

#include "base/types.h"

namespace base {

namespace opengl { class DeviceContext; }

class Engine {
public:
    ~Engine();

    static void init(opengl::DeviceContext& context);
    static void shutdown();
private:
    Engine(opengl::DeviceContext& context);

    opengl::DeviceContext& GL;
    static Engine* instance_;
};

} // namespace base