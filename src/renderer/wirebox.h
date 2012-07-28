/**
 * \file
 * \brief       Wire Box
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/

#include "glcontext.h"
#include "base/math/vector.h"
#include "glprogram.h"

namespace base   {
namespace opengl {

class WireBox
{
public:
    void setMinPoint(const math::Vector3& minPoint);
    void setMaxPoint(const math::Vector3& maxPoint);
    void setMinMaxPoints(const math::Vector3& minPoint, const math::Vector3& maxPoint);
    const math::Vector3& getMinPoint() const;
    const math::Vector3& getMaxPoint() const;
private:
    math::Vector3 minPoint_;
    math::Vector3 maxPoint_;
    math::Vector3 points[8];

public:
    WireBox();
    WireBox(math::Vector3 minPoint, math::Vector3 maxPoin);
    ~WireBox();
    void Draw(Program* program);
private:
    void update();
};


}
}
