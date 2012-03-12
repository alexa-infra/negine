#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include "base/math/matrix.h"

namespace base {
namespace opengl {

class Camera {
private:
    base::math::Matrix4 orientation_;


public:

    base::math::Vector3 forward;
    base::math::Vector3 right;
    base::math::Vector3 up;
    
    base::math::Vector3 position;
    
    f32 pitch;
    f32 head;
        
    f32 aspect;
    f32 fov;
    f32 zNear;
    f32 zFar;

    Camera();

    base::math::Matrix4 GetProjection() const;
    base::math::Matrix4 GetModelView() const;

    void UpdateOrientation();
private:
    void UpdateVectors();
private:
    DISALLOW_COPY_AND_ASSIGN(Camera);
};

}
}