/**
 * \file
 * \brief       Camera class, provides projection and modelview matrix
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "base/math/vector.h"
#include "base/math/matrix.h"

namespace base {
namespace opengl {

using base::math::Matrix4;
using base::math::Vector3;

class Camera {
private:
    Matrix4 orientation_;

public:

    Vector3 forward;
    Vector3 right;
    Vector3 up;
    
    Vector3 position;
    
    f32 pitch;
    f32 head;
        
    f32 aspect;
    f32 fov;
    f32 zNear;
    f32 zFar;

    Camera();

    Matrix4 GetProjection() const;
    Matrix4 GetModelView() const;

    void UpdateOrientation();
private:
    void UpdateVectors();
private:
    DISALLOW_COPY_AND_ASSIGN(Camera);
};

}
}