#pragma once

#include "render/glcontext.h"

namespace base
{
namespace opengl
{

class NEGINE_API GpuResource
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