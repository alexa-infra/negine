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
    : numAnimatedComponents( 0 )
    , numFrames( 0 )
    , numJoints( 0 )
    , frameRate( 0 )
{
}

Md5Anim::~Md5Anim()
{
}

void Md5Anim::load( const std::string& filename )
{
    Lexer reader( filename );

    while( reader.hasMoreData() ) {
        reader.readToken();

        // TODO: strcmp(...) == 0 move to equal<T>
        if ( strcmp( reader.currentToken(), "MD5Version" ) == 0 ) {
            u32 version = static_cast<u32>(reader.readFloat());

            if ( version != 10 ) {
                return;
            }
        } else if ( strcmp( reader.currentToken(), "numFrames" ) == 0 ) {
            numFrames = static_cast<u32>(reader.readFloat());

            // Allocate memory for skeleton frames and bounding boxes
            skelFrames.resize(numFrames);
            bboxes.resize(numFrames);
        } else if ( strcmp( reader.currentToken(), "numJoints" ) == 0 ) {
            numJoints = static_cast<u32>(reader.readFloat());

            skelFrames.resize(numJoints * numFrames);

            jointInfos.resize(numJoints);
            baseFrame.resize(numJoints);
        } else if ( strcmp( reader.currentToken(), "frameRate" ) == 0 ) {
            frameRate = static_cast<u32>(reader.readFloat());
        } else if ( strcmp( reader.currentToken(), "numAnimatedComponents" ) == 0 ) {
            numAnimatedComponents = static_cast<u32>(reader.readFloat());

            animFrameData.resize(numAnimatedComponents);
        } else if ( strcmp( reader.currentToken(), "hierarchy" ) == 0 ) {
            reader.readToken(); // {

            for ( u32 i = 0; i < numJoints; i++ ) {
                jointInfos[i].name        = reader.readToken();
                jointInfos[i].parent      = static_cast<u32>(reader.readFloat());
                jointInfos[i].flags       = static_cast<u32>(reader.readFloat());
                jointInfos[i].startIndex  = static_cast<u32>(reader.readFloat());
            }

            reader.readToken(); // }
        } else if ( strcmp( reader.currentToken(), "bounds" ) == 0 ) {
            reader.readToken(); // {

            for ( u32 i = 0; i < numFrames; i++ ) {
                reader.readToken(); // (
                readVector( reader, bboxes[i].min );
                reader.readToken(); // )
                reader.readToken(); // (
                readVector( reader, bboxes[i].max );
                reader.readToken(); // )
            }

            reader.readToken(); // }
        } else if ( strcmp( reader.currentToken(), "baseframe" ) == 0 ) {
            reader.readToken(); // {

            for ( u32 i = 0; i < numJoints; i++ ) {
                reader.readToken(); // (
                readVector( reader, baseFrame[i].pos );
                reader.readToken(); // )
                reader.readToken(); // (
                readQuat( reader, baseFrame[i].orient );
                baseFrame[i].orient.ComputeW();
                reader.readToken(); // )
            }

            reader.readToken(); // }
        } else if ( strcmp( reader.currentToken(), "frame" ) == 0 ) {
            u32 frameIndex = static_cast<u32>(reader.readFloat());
            reader.readToken(); // {

            for ( u32 i = 0; i < numAnimatedComponents; ++i ) {
                animFrameData[i] = reader.readFloat();
            }

            reader.readToken(); // }
            buildFrameSkeleton( frameIndex );
        }
    }
}

void Md5Anim::buildFrameSkeleton( u32 frameIndex )
{
    for ( u32 i = 0; i < numJoints; ++i ) {
        const BaseframeJoint& baseJoint = baseFrame[i];
        math::vec3f animatedPos;
        math::Quat  animatedOrient;
        i32 j = 0;
        animatedPos    = baseJoint.pos;
        animatedOrient = baseJoint.orient;

        i32 startIndex = jointInfos[i].startIndex;
        if ( jointInfos[i].flags & 1 ) { //Tx
            animatedPos[0] = animFrameData[startIndex + j];
            ++j;
        }

        if ( jointInfos[i].flags & 2 ) { //Ty
            animatedPos[1] = animFrameData[startIndex + j];
            ++j;
        }

        if ( jointInfos[i].flags & 4 ) { //Tz
            animatedPos[2] = animFrameData[startIndex + j];
            ++j;
        }

        if ( jointInfos[i].flags & 8 ) { //Qx
            animatedOrient.x = animFrameData[startIndex + j];
            ++j;
        }

        if ( jointInfos[i].flags & 16 ) { //Qy
            animatedOrient.y = animFrameData[startIndex + j];
            ++j;
        }

        if ( jointInfos[i].flags & 32 ) { //Qz
            animatedOrient.z = animFrameData[startIndex + j];
            ++j;
        }

        animatedOrient.ComputeW();
        Md5Joint& thisJoint = skelFrames[numJoints * frameIndex + i];
        i32 parent          = jointInfos[i].parent;
        thisJoint.parent    = parent;
        thisJoint.name      = jointInfos[i].name;

        if ( thisJoint.parent < 0 ) {
            thisJoint.pos    = animatedPos;
            thisJoint.orient = animatedOrient;
        } else {
            Md5Joint& parentJoint = skelFrames[numJoints * frameIndex + parent];
            math::vec3f rotatedPos = parentJoint.orient.RotatePoint( animatedPos );
            thisJoint.pos = rotatedPos + parentJoint.pos;
            thisJoint.orient = parentJoint.orient * animatedOrient;
            thisJoint.orient.Normalize();
        }
    }
}

void Md5Anim::update( Md5Model* model, u32 frameIndex, f32 interpPhase )
{
    Md5Joint* previousFrame = &skelFrames[(frameIndex % numFrames) * numJoints];
    Md5Joint* nextFrame     = &skelFrames[(( frameIndex + 1 ) % numFrames) * numJoints];

    for ( u32 i = 0; i < numJoints; ++i ) {
        model->baseSkel[i].parent = previousFrame[i].parent;
        model->baseSkel[i].pos = previousFrame[i].pos + ( nextFrame[i].pos - previousFrame[i].pos ) * interpPhase;
        model->baseSkel[i].orient = math::Quat::GetSlerp( previousFrame[i].orient, nextFrame[i].orient, interpPhase );
    }
}

}
}
