#pragma once

#include "render/glcontext.h"

namespace base
{
namespace opengl
{

class GpuResource
{
public:
    GpuResource(DeviceContext& context);
    GLuint handle();
protected:
    DeviceContext& GL;
    GLuint id_;
};

}
}