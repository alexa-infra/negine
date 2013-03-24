/**
 * \file
 * \brief       Wire Box
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/

#include "render/wirebox.h"
#include "render/statistics.h"
#include "render/gpuprogram.h"

using base::math::Vector3;
using base::math::Vector4;

namespace base
{
namespace opengl
{

//Lifecyrcle
WireBox::WireBox()
{
    minPoint_ = Vector3( 0.0f, 0.0f, 0.0f );
    maxPoint_ = Vector3( 0.0f, 0.0f, 0.0f );
    update();
}

WireBox::WireBox( Vector3 minPoint, Vector3 maxPoint )
{
    minPoint_ = minPoint;
    maxPoint_ = maxPoint;
    update();
}

WireBox::~WireBox()
{
}

//Setters & Getters
void WireBox::setMinPoint( const Vector3& minPoint )
{
    minPoint_ = minPoint;
    update();
}

void WireBox::setMaxPoint( const Vector3& maxPoint )
{
    maxPoint_ = maxPoint;
    update();
}

void WireBox::setMinMaxPoints( const Vector3& minPoint, const Vector3& maxPoint )
{
    minPoint_ = minPoint;
    maxPoint_ = maxPoint;
    update();
}

const Vector3& WireBox::getMinPoint() const
{
    return minPoint_;
}

const Vector3& WireBox::getMaxPoint() const
{
    return maxPoint_;
}

//Public methods
void WireBox::Draw( GpuProgram* program )
{
    program->set_uniform( "color", Vector4( 1, 0, 0, 1 ) );
    u32 bindPos = VertexAttrs::GetAttributeLocation(VertexAttrs::tagPosition);
    //glVertexAttribPointer(
    //    bindPos,
    //    3,
    //    GL_FLOAT,
    //    GL_FALSE,
    //    sizeof( Vector3 ),
    //    ( u8* )points );
    //u32 indexes[] = { 0, 1, 1, 3, 3, 7, 7, 0, 0, 5, 1, 2, 3, 4, 7, 6, 5, 2, 2, 4, 4, 6, 6, 5 };
    //glDrawElements( GL_LINES, 24, GL_UNSIGNED_INT, indexes );
    Stats::add_polygons( 12 );
}

void WireBox::update()
{
    points[0] = Vector3( minPoint_.x, minPoint_.y, minPoint_.z );
    points[1] = Vector3( maxPoint_.x, minPoint_.y, minPoint_.z );
    points[2] = Vector3( maxPoint_.x, maxPoint_.y, minPoint_.z );
    points[3] = Vector3( maxPoint_.x, minPoint_.y, maxPoint_.z );
    points[4] = Vector3( maxPoint_.x, maxPoint_.y, maxPoint_.z );
    points[5] = Vector3( minPoint_.x, maxPoint_.y, minPoint_.z );
    points[6] = Vector3( minPoint_.x, maxPoint_.y, maxPoint_.z );
    points[7] = Vector3( minPoint_.x, minPoint_.y, maxPoint_.z );
}

}
}
