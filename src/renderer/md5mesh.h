/**
 * \file
 * \brief       Md5 mesh loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "mesh.h"
#include "base/math/vector.h"
#include "base/math/quat.h"

namespace base {
namespace opengl {

struct Md5Joint
{
    string name;
    i32 parent;
    math::Vector3 pos;
    math::Quat orient;
};

struct Md5Vertex
{
    math::Vector2 st;

    i32 start;
    i32 count;
};

struct Md5Triangle
{
    i32 index[3];
};

struct Md5Weight
{
    i32 joint;
    f32 bias;

    math::Vector3 pos;

    math::Vector3 normal;
    math::Vector3 tangent;
};

struct Md5BoundingBox
{
    math::Vector3 min;
    math::Vector3 max;
};

struct Md5Mesh
{
    Md5Vertex* vertices;
    Md5Triangle* triangles;
    Md5Weight* weights;

    i32 num_verts;
    i32 num_tris;
    i32 num_weights;

    string shader;
    string bumpShader;

    Vertex* vertexArray;
    Face* vertexIndices;
};

struct Md5Model
{
    Md5Joint* baseSkel;
    Md5Mesh* meshes;

    i32 num_joints;
    i32 num_meshes;
};

class Md5Anim
{
private:
    Md5Anim();
public:
    i32 num_frames;
    i32 num_joints;
    i32 frameRate;

    Md5Joint** skelFrames;
    Md5BoundingBox* bboxes;

    ~Md5Anim();

    static Md5Anim* Load(const string& filename);
};

class Md5Object
{
public:
    Md5Model md5Model;
    Md5Anim  md5Anim;

    bool animated;

    void Update();
};

class Material;

class Entity
{
private:
    Entity();
public:
    ~Entity();

    static Entity* Load(const string& filename);

    Material* material;
    Md5Object* object;
    math::Matrix4 transform;
};

// void GenerateGPUVertices (md5_mesh_t *mesh, const md5_joint_t *skeleton);
// void GenerateLightingInfo (const md5_mesh_t *mesh,  md5_joint_t *skeleton);
// void DrawSkeleton (const md5_joint_t *skeleton, int num_joints);

// int CheckAnimValidity (const md5_model_t *mdl,const md5_anim_t *anim);
// void InterpolateSkeletons (int frameIndex,int numFrames, int num_joints, float interp,  md5_joint_t **joints, md5_joint_t *targetJoints);

}
}