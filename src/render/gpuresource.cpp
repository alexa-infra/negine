#include "render/gpuresource.h"

namespace base
{
namespace opengl
{

GpuResource::GpuResource(DeviceContext& context)
    : GL(context)
    , id_( 0 )
{
}

GLuint GpuResource::handle()
{
    return id_;
}

}
}