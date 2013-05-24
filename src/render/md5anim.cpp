/**
 * \file
 * \brief       Md5 mesh animation loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/

#include "md5anim.h"
#include <string.h>
#include "base/lexer.h"
#include "math/quat.h"

namespace base
{
namespace resource
{

Md5Anim::Md5Anim()
    : jointInfos( NULL )
    , baseFrame( NULL )
    , numAnimatedComponents( 0 )
    , animFrameData( NULL )
    , num_frames( 0 )
    , num_joints( 0 )
    , frame_rate( 0 )
    , skelFrames( NULL )
    , bboxes( NULL )
{
}

Md5Anim::~Md5Anim()
{
    delete[] jointInfos;
    delete[] baseFrame;
    delete[] animFrameData;
    delete[] bboxes;

    for ( u32 i = 0; i < num_frames; ++i ) {
        delete[] skelFrames[i];
    }

    delete[] skelFrames;
}

void Md5Anim::Load( const std::string& filename )
{
    Lexer reader( filename );

    while( reader.HasMoreData() ) {
        reader.ReadToken();

        if ( strcmp( reader.CurrentToken(), "MD5Version" ) == 0 ) {
            u32 version = ( u32 ) reader.ReadFloat();

            if ( version != 10 ) {
                return;
            }
        } else if ( strcmp( reader.CurrentToken(), "numFrames" ) == 0 ) {
            num_frames = ( u32 ) reader.ReadFloat();

            // Allocate memory for skeleton frames and bounding boxes
            if ( num_frames > 0 ) {
                skelFrames = new Md5Joint*      [num_frames];
                bboxes     = new Md5BoundingBox [num_frames];
            }
        } else if ( strcmp( reader.CurrentToken(), "numJoints" ) == 0 ) {
            num_joints = ( u32 ) reader.ReadFloat();

            for ( u32 i = 0; i < num_frames; ++i ) {
                // Allocate memory for joints of each frame
                skelFrames[i] = new Md5Joint[num_joints];
            }

            jointInfos = new JointInfo     [num_joints];
            baseFrame  = new BaseframeJoint[num_joints];
        } else if ( strcmp( reader.CurrentToken(), "frameRate" ) == 0 ) {
            frame_rate = ( u32 ) reader.ReadFloat();
        } else if ( strcmp( reader.CurrentToken(), "numAnimatedComponents" ) == 0 ) {
            numAnimatedComponents = ( u32 ) reader.ReadFloat();

            if ( numAnimatedComponents > 0 ) {
                // Allocate memory for animation frame data
                animFrameData = new f32[numAnimatedComponents];
            }
        } else if ( strcmp( reader.CurrentToken(), "hierarchy" ) == 0 ) {
            reader.ReadToken(); // {

            for ( u32 i = 0; i < num_joints; i++ ) {
                jointInfos[i].name        = reader.ReadToken();
                jointInfos[i].parent      = ( u32 )reader.ReadFloat();
                jointInfos[i].flags       = ( u32 )reader.ReadFloat();
                jointInfos[i].start_index = ( u32 )reader.ReadFloat();
            }

            reader.ReadToken(); // }
        } else if ( strcmp( reader.CurrentToken(), "bounds" ) == 0 ) {
            reader.ReadToken(); // {

            for ( u32 i = 0; i < num_frames; i++ ) {
                reader.ReadToken(); // (
                readVector( reader, bboxes[i].min );
                reader.ReadToken(); // )
                reader.ReadToken(); // (
                readVector( reader, bboxes[i].max );
                reader.ReadToken(); // )
            }

            reader.ReadToken(); // }
        } else if ( strcmp( reader.CurrentToken(), "baseframe" ) == 0 ) {
            reader.ReadToken(); // {

            for ( u32 i = 0; i < num_joints; i++ ) {
                reader.ReadToken(); // (
                readVector( reader, baseFrame[i].pos );
                reader.ReadToken(); // )
                reader.ReadToken(); // (
                readQuat( reader, baseFrame[i].orient );
                baseFrame[i].orient.ComputeW();
                reader.ReadToken(); // )
            }

            reader.ReadToken(); // }
        } else if ( strcmp( reader.CurrentToken(), "frame" ) == 0 ) {
            u32 frameIndex = ( u32 ) reader.ReadFloat();
            reader.ReadToken(); // {

            for ( u32 i = 0; i < numAnimatedComponents; ++i ) {
                animFrameData[i] = reader.ReadFloat();
            }

            reader.ReadToken(); // }
            BuildFrameSkeleton( frameIndex );
        }
    }
}

void Md5Anim::BuildFrameSkeleton( u32 frameIndex )
{
    for ( u32 i = 0; i < num_joints; ++i ) {
        const BaseframeJoint& baseJoint = baseFrame[i];
        math::vec3f animatedPos;
        math::Quat    animatedOrient;
        i32 j = 0;
        animatedPos    = baseJoint.pos;
        animatedOrient = baseJoint.orient;

        if ( jointInfos[i].flags & 1 ) { //Tx
            animatedPos[0] = animFrameData[jointInfos[i].start_index + j];
            ++j;
        }

        if ( jointInfos[i].flags & 2 ) { //Ty
            animatedPos[1] = animFrameData[jointInfos[i].start_index + j];
            ++j;
        }

        if ( jointInfos[i].flags & 4 ) { //Tz
            animatedPos[2] = animFrameData[jointInfos[i].start_index + j];
            ++j;
        }

        if ( jointInfos[i].flags & 8 ) { //Qx
            animatedOrient.x = animFrameData[jointInfos[i].start_index + j];
            ++j;
        }

        if ( jointInfos[i].flags & 16 ) { //Qy
            animatedOrient.y = animFrameData[jointInfos[i].start_index + j];
            ++j;
        }

        if ( jointInfos[i].flags & 32 ) { //Qz
            animatedOrient.z = animFrameData[jointInfos[i].start_index + j];
            ++j;
        }

        animatedOrient.ComputeW();
        Md5Joint& thisJoint = skelFrames[frameIndex][i];
        i32 parent          = jointInfos[i].parent;
        thisJoint.parent    = parent;
        thisJoint.name      = jointInfos[i].name;

        if ( thisJoint.parent < 0 ) {
            thisJoint.pos    = animatedPos;
            thisJoint.orient = animatedOrient;
        } else {
            Md5Joint& parentJoint = skelFrames[frameIndex][parent];
            math::vec3f rotatedPos = parentJoint.orient.RotatePoint( animatedPos );
            thisJoint.pos = rotatedPos + parentJoint.pos;
            thisJoint.orient = parentJoint.orient * animatedOrient;
            thisJoint.orient.Normalize();
        }
    }
}

void Md5Anim::Update( Md5Model* model, u32 frameIndex, f32 interp_phase )
{
    Md5Joint* previousFrame = skelFrames[frameIndex % num_frames];
    Md5Joint* nextFrame     = skelFrames[( frameIndex + 1 ) % num_frames];

    for ( u32 i = 0; i < num_joints; ++i ) {
        model->baseSkel[i].parent = previousFrame[i].parent;
        model->baseSkel[i].pos = previousFrame[i].pos + ( nextFrame[i].pos - previousFrame[i].pos ) * interp_phase;
        model->baseSkel[i].orient = math::Quat::GetSlerp( previousFrame[i].orient, nextFrame[i].orient, interp_phase );
    }
}

}
}
