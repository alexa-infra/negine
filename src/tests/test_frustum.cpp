/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \brief       Test for frustum intersection with OBB
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "renderer/camera.h"

using base::opengl::Camera;
using base::math::Plane;
using base::math::Matrix4;
using base::math::Vector3;
using base::math::pi;

const Vector3 r(1, 0, 0);
const Vector3 u(0, 1, 0);
const Vector3 f(0, 0, 1);

TEST(CameraFrustum, Basic) {

    Camera cam;
    cam.set_position(Vector3(0, 0, 0));
    cam.set_head(0);
    cam.set_pitch(0);
    cam.set_aspect(1);
    cam.set_fov(90);
    cam.set_zNear(1);
    cam.set_zFar(1000);

    cam.Update();

    EXPECT_EQ(cam.forward(), f);
    EXPECT_EQ(cam.up(), u);
    EXPECT_EQ(cam.right(), r);

    const Vector3 p(0.0f, 0.0f, 20.f);
    for (int i=0; i<6; i++)
    {
        std::cout << i << std::endl;
        EXPECT_TRUE( cam.planes()[i].Distance(p) > 0 );
    }
}

TEST(CameraFrustum, Planes)
{
    const Plane p(0, 0, 1, 0);
    
    EXPECT_EQ(p.BoxOnPlaneSide(Vector3(1, 1, 1), Vector3(5, 5, 5)), 1);
    EXPECT_EQ(p.BoxOnPlaneSide(Vector3(-5, -5, -5), Vector3(-1, -1, -1)), 2);
    EXPECT_EQ(p.BoxOnPlaneSide(Vector3(-5, -5, -5), Vector3(1, 1, 1)), 3);
    EXPECT_EQ(p.BoxOnPlaneSide(Vector3(1, 1, 1), Vector3(-5, -5, -5)), 0);
}
