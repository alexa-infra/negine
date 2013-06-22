/**
 * \file
 * \brief       Md5 mesh loader
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/quat.h"
#include <string>
#include <vector>

namespace base
{

class Lexer;

namespace resource
{

class Md5Anim;

struct Md5Joint {
    std::string name;
    i32 parent;
    math::vec3f pos;
    math::Quat orient;

    math::vec3f translate( const math::vec3f& v ) const;
};

struct Md5Vertex {
    math::vec2f st;

    u32 start;
    u32 count;
};

struct Md5Triangle {
    u32 index[3];
};

struct Md5Weight {
    i32 joint;
    f32 bias;

    math::vec3f pos;

    math::vec3f normal;
    math::vec3f tangent;
};

struct Md5BoundingBox {
    math::vec3f min;
    math::vec3f max;
};

struct Md5Mesh
{
    std::vector<Md5Vertex>   vertices;
    std::vector<Md5Triangle> triangles;
    std::vector<Md5Weight>   weights;

    i32 numVerts;
    i32 numTris;
    i32 numWeights;

    std::string shader;

    void Read( Lexer& reader );
};

struct Md5Model {
    std::vector<Md5Joint> baseSkel;
    std::vector<Md5Mesh>  meshes;

    void Read( Lexer& reader );
};

class Md5Object
{
public:
    Md5Model md5Model;
    Md5Anim*  md5Anim;

    bool animated;

    void Update();
};

class Material;

class Entity
{
private:
    //Material* material;

    Entity() {}
public:
    ~Entity() {}

    static Entity* Load( const std::string& filename );

    Md5Object object;
    math::Matrix4 transform;
};

void readVector( Lexer& reader, math::vec3f& vec );
void readQuat( Lexer& reader, math::Quat& q );

}
}
