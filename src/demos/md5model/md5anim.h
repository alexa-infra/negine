/**
 * \file
 * \brief       Md5 mesh animation loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "md5mesh.h"
#include "math/vec3.h"
#include "math/quat.h"

namespace base
{
namespace resource
{

//! Joint info
struct JointInfo {
    std::string name;
    i32 parent;
    i32 flags;
    i32 startIndex;
};

//! Base frame joint
struct BaseframeJoint {
    math::vec3f pos;
    math::Quat orient;
};

class Md5Anim
{
public:
    std::vector<Md5Joint> skelFrames;
    std::vector<Md5BoundingBox> bboxes;

    Md5Anim();
    ~Md5Anim();

    void load( const std::string& filename );
    void update( Md5Model* model, u32 frameIndex, f32 interpPhase );
private:
    void buildFrameSkeleton( u32 frameIndex );
    
private:
    u32 numFrames;
    u32 numJoints;
    u32 frameRate;

    std::vector<JointInfo> jointInfos;
    std::vector<BaseframeJoint> baseFrame;
    std::vector<f32> animFrameData;
    u32 numAnimatedComponents;
};


}
}
