/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "engine/resource.h"
#include "render/mesh.h"
#include <vector>

namespace base {
namespace opengl {

class Model : public ResourceBase<Model> {
public:
    NEGINE_API Model();
    NEGINE_API ~Model();

    struct Surface {
        Mesh mesh;
        //std::string name;
        //math::Matrix4 transform;
        //std::string material;
        //Params params;
        u32 vertexStart;    //! bytes
        u32 indexStart;     //! bytes
    };

    NEGINE_API size_t surfaceCount() const;
    NEGINE_API const Surface& surfaceAt(size_t i) const;
    NEGINE_API Surface& beginSurface();
    NEGINE_API void endSurface();
    NEGINE_API void done();
private:
    Surface* currentSurface_;
    std::vector<Surface> surfaces_;
    u32 vertexSize_;
    u32 indexSize_;
};

} // namespace opengl
} // namespace base
