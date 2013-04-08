/**
 * \file
 * \brief       Wire Box
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "math/vec3.h"

namespace base
{
namespace opengl
{

class GpuProgram;

class WireBox
{
public:
    void setMinPoint( const math::vec3f& minPoint );
    void setMaxPoint( const math::vec3f& maxPoint );
    void setMinMaxPoints( const math::vec3f& minPoint, const math::vec3f& maxPoint );
    const math::vec3f& getMinPoint() const;
    const math::vec3f& getMaxPoint() const;
private:
    math::vec3f minPoint_;
    math::vec3f maxPoint_;
    math::vec3f points[8];

public:
    WireBox();
    WireBox( math::vec3f minPoint, math::vec3f maxPoin );
    ~WireBox();
    void Draw( GpuProgram* program );
private:
    void update();
};


}
}
