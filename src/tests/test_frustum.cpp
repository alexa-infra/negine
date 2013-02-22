/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \brief       Test for frustum intersection with OBB
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "render/camera.h"

using base::opengl::Camera;
using base::math::Plane;
using base::math::Matrix4;
using base::math::Vector3;
using base::math::pi;

const Vector3 r( 1, 0, 0 );
const Vector3 u( 0, 1, 0 );
const Vector3 f( 0, 0, -1 );

TEST( CameraFrustum, Basic )
{
    Camera cam;
    cam.set_position( Vector3( 0, 0, 0 ) );
    cam.set_head( 0 );
    cam.set_pitch( 0 );
    cam.set_aspect( 1 );
    cam.set_fov( 90 );
    cam.set_zNear( 1 );
    cam.set_zFar( 1000 );
    cam.Update();
    EXPECT_EQ( cam.forward(), f );
    EXPECT_EQ( cam.up(), u );
    EXPECT_EQ( cam.right(), r );
    const Vector3 p( 0.0f, 0.0f, -20.f );

    EXPECT_TRUE( cam.planes()[0].Distance( p ) > 0 );
    EXPECT_TRUE( cam.planes()[1].Distance( p ) > 0 );
    EXPECT_TRUE( cam.planes()[2].Distance( p ) > 0 );
    EXPECT_TRUE( cam.planes()[3].Distance( p ) > 0 );
    EXPECT_TRUE( cam.planes()[4].Distance( p ) > 0 );
    EXPECT_TRUE( cam.planes()[5].Distance( p ) > 0 );
}

TEST( CameraFrustum, PlaneDistance )
{
    const Plane p( 0, 0, 1, -10 );
    EXPECT_FLOAT_EQ( 5.0f, p.Distance( Vector3(0, 0, 15) ) );
    EXPECT_FLOAT_EQ( -5.0f, p.Distance( Vector3(0, 0, 5) ) );
    EXPECT_FLOAT_EQ( 0.0f, p.Distance( Vector3(0, 0, 10) ) );

    const Plane p1( 0, 0, 2, -10 );
    EXPECT_FLOAT_EQ( 10.0f, p1.Distance( Vector3(0, 0, 15) ) );
    EXPECT_FLOAT_EQ( -10.0f, p1.Distance( Vector3(0, 0, -5) ) );
    EXPECT_FLOAT_EQ( 5.0f, p1.Distance( Vector3(0, 0, 10) ) );

    const Plane p2( 0, 0, 1, 10 );
    EXPECT_FLOAT_EQ( 15.0f, p2.Distance( Vector3(0, 0, 5) ) );
    EXPECT_FLOAT_EQ( 5.0f, p2.Distance( Vector3(0, 0, -5) ) );
    EXPECT_FLOAT_EQ( -5.0f, p2.Distance( Vector3(0, 0, -15) ) );

    const Plane p3( 0, 0, -1, -10 );
    EXPECT_FLOAT_EQ( -15.0f, p3.Distance( Vector3(0, 0, 5) ) );
    EXPECT_FLOAT_EQ( -5.0f, p3.Distance( Vector3(0, 0, -5) ) );
    EXPECT_FLOAT_EQ( 5.0f, p3.Distance( Vector3(0, 0, -15) ) );
}

TEST( CameraFrustum, Planes )
{
    const Plane p( 0, 0, 1, -10 );
    EXPECT_EQ( 0, p.BoxOnPlaneSide( Vector3( 1, 1, 1 ),     Vector3( 15, 15, 15 )      ) );
    EXPECT_EQ( 1, p.BoxOnPlaneSide( Vector3( 15, 15, 15 ),  Vector3( 20, 20, 20 )   ) );
    EXPECT_EQ( 2, p.BoxOnPlaneSide( Vector3( 0, 0, 0 ),  Vector3( 1, 1, 1 )      ) );

    const Plane p1( 0, 0, -1, 10 );
    EXPECT_EQ( 0, p1.BoxOnPlaneSide( Vector3( 1, 1, 1 ),     Vector3( 15, 15, 15 )      ) );
    EXPECT_EQ( 2, p1.BoxOnPlaneSide( Vector3( 15, 15, 15 ),  Vector3( 20, 20, 20 )   ) );
    EXPECT_EQ( 1, p1.BoxOnPlaneSide( Vector3( 0, 0, 0 ),  Vector3( 1, 1, 1 )      ) );

    const Plane p2( 0, 0, 1, 10 );
    EXPECT_EQ( 0, p2.BoxOnPlaneSide( Vector3( -15, -15, -15 ),     Vector3( 15, 15, 15 )      ) );
    EXPECT_EQ( 1, p2.BoxOnPlaneSide( Vector3( 15, 15, 15 ),  Vector3( 20, 20, 20 )   ) );
    EXPECT_EQ( 2, p2.BoxOnPlaneSide( Vector3( -15, -15, -15 ),  Vector3( -16, -16, -16 )      ) );

    const Plane p3( 0, 0, -1, -10 );
    EXPECT_EQ( 0, p3.BoxOnPlaneSide( Vector3( -15, -15, -15 ),     Vector3( 15, 15, 15 )      ) );
    EXPECT_EQ( 2, p3.BoxOnPlaneSide( Vector3( 15, 15, 15 ),  Vector3( 20, 20, 20 )   ) );
    EXPECT_EQ( 1, p3.BoxOnPlaneSide( Vector3( -15, -15, -15 ),  Vector3( -16, -16, -16 )      ) );

}
