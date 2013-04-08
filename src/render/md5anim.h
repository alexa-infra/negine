/**
 * \file
 * \brief       Md5 mesh animation loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/md5mesh.h"
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
    i32 start_index;
};

//! Base frame joint
struct BaseframeJoint {
    math::vec3f pos;
    math::Quat    orient;
};

class Md5Anim
{
private:
    JointInfo* jointInfos;
    BaseframeJoint* baseFrame;
    u32 numAnimatedComponents;
    f32* animFrameData;

public:
    u32 num_frames;
    u32 num_joints;
    u32 frame_rate;

    Md5Joint** skelFrames;
    Md5BoundingBox* bboxes;

    Md5Anim();
    ~Md5Anim();

    void Load( const std::string& filename );
    void BuildFrameSkeleton( u32 frameIndex );

    void Update( Md5Model* model, u32 frameIndex, f32 interp_phase );
};


}
}
