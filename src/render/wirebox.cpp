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
#include "math/vec4.h"

using base::math::vec3f;
using base::math::vec4f;

namespace base
{
namespace opengl
{

//Lifecyrcle
WireBox::WireBox()
{
    minPoint_ = vec3f( 0.0f, 0.0f, 0.0f );
    maxPoint_ = vec3f( 0.0f, 0.0f, 0.0f );
    update();
}

WireBox::WireBox( vec3f minPoint, vec3f maxPoint )
{
    minPoint_ = minPoint;
    maxPoint_ = maxPoint;
    update();
}

WireBox::~WireBox()
{
}

//Setters & Getters
void WireBox::setMinPoint( const vec3f& minPoint )
{
    minPoint_ = minPoint;
    update();
}

void WireBox::setMaxPoint( const vec3f& maxPoint )
{
    maxPoint_ = maxPoint;
    update();
}

void WireBox::setMinMaxPoints( const vec3f& minPoint, const vec3f& maxPoint )
{
    minPoint_ = minPoint;
    maxPoint_ = maxPoint;
    update();
}

const vec3f& WireBox::getMinPoint() const
{
    return minPoint_;
}

const vec3f& WireBox::getMaxPoint() const
{
    return maxPoint_;
}

//Public methods
void WireBox::Draw( GpuProgram* program )
{
    program->set_uniform( "color", vec4f( 1, 0, 0, 1 ) );
    u32 bindPos = VertexAttrs::GetAttributeLocation(VertexAttrs::tagPosition);
    //glVertexAttribPointer(
    //    bindPos,
    //    3,
    //    GL_FLOAT,
    //    GL_FALSE,
    //    sizeof( vec3f ),
    //    ( u8* )points );
    //u32 indexes[] = { 0, 1, 1, 3, 3, 7, 7, 0, 0, 5, 1, 2, 3, 4, 7, 6, 5, 2, 2, 4, 4, 6, 6, 5 };
    //glDrawElements( GL_LINES, 24, GL_UNSIGNED_INT, indexes );
    Stats::add_polygons( 12 );
}

void WireBox::update()
{
    points[0] = vec3f( minPoint_.x, minPoint_.y, minPoint_.z );
    points[1] = vec3f( maxPoint_.x, minPoint_.y, minPoint_.z );
    points[2] = vec3f( maxPoint_.x, maxPoint_.y, minPoint_.z );
    points[3] = vec3f( maxPoint_.x, minPoint_.y, maxPoint_.z );
    points[4] = vec3f( maxPoint_.x, maxPoint_.y, maxPoint_.z );
    points[5] = vec3f( minPoint_.x, maxPoint_.y, minPoint_.z );
    points[6] = vec3f( minPoint_.x, maxPoint_.y, maxPoint_.z );
    points[7] = vec3f( minPoint_.x, minPoint_.y, maxPoint_.z );
}

}
}
