#include "md5mesh.h"
#include "base/lexer.h"
#include <string.h>
#include "base/debug.h"

namespace base
{
namespace resource
{

math::vec3f Md5Joint::translate( const math::vec3f& v ) const
{
    return orient.RotatePoint( v ) + pos;
}

void readVector( Lexer& reader, math::vec3f& vec )
{
    vec.x = reader.readFloat();
    vec.y = reader.readFloat();
    vec.z = reader.readFloat();
}

void readQuat( Lexer& reader, math::Quat& q )
{
    q.x = reader.readFloat();
    q.y = reader.readFloat();
    q.z = reader.readFloat();
}

Entity* Entity::Load( const std::string& filename )
{
    Lexer reader( filename );
    Entity* entity = new Entity;
    Md5Model& model = entity->object.md5Model;
    model.Read( reader );
    return entity;
}

void Md5Model::Read( Lexer& reader )
{
    u32 currentMeshIndex = 0;

    while( reader.hasMoreData() ) {
        reader.readToken();

        if ( strcmp( reader.currentToken(), "MD5Version" ) == 0 ) {
            u32 version = static_cast<u32>(reader.readFloat());

            ASSERT( version == 10 );
        } else if ( strcmp( reader.currentToken(), "numJoints" ) == 0 ) {
            u32 numJoints = static_cast<u32>(reader.readFloat());

            if ( numJoints > 0 ) {
                baseSkel.resize(numJoints);
            }
        } else if ( strcmp( reader.currentToken(), "numMeshes" ) == 0 ) {
            u32 numMeshes = static_cast<u32>(reader.readFloat());

            if ( numMeshes > 0 ) {
                meshes.resize(numMeshes);
            }
        } else if ( strcmp( reader.currentToken(), "joints" ) == 0 ) {
            reader.readToken(); // {

            for ( i32 i = 0; i < baseSkel.size(); ++i ) {
                Md5Joint& joint = baseSkel[i];
                //"name parent ( pos.x pos.y pos.z ) ( q.x q.y q.z )"
                joint.name = reader.readToken();
                joint.parent = static_cast<i32>(reader.readFloat());
                reader.readToken(); // (
                readVector( reader, joint.pos );
                reader.readToken(); // )
                reader.readToken(); // (
                readQuat( reader, joint.orient );
                reader.readToken(); // )
                joint.orient.ComputeW();
            }

            reader.readToken(); // }
        } else if ( strcmp( reader.currentToken(), "mesh" ) == 0 ) {
            reader.readToken(); // {
            Md5Mesh& mesh = meshes[currentMeshIndex];
            mesh.Read( reader );
            currentMeshIndex++;
        }
    }
}

void Md5Mesh::Read( Lexer& reader )
{
    while ( strcmp( reader.currentToken(), "}" ) != 0 && reader.hasMoreData() ) {
        reader.readToken();

        if ( strcmp( reader.currentToken(), "shader" ) == 0 ) {
            shader = reader.readToken();
        } else if ( strcmp( reader.currentToken(), "numverts" ) == 0 ) {
            u32 numVerts = static_cast<u32>(reader.readFloat());

            if ( numVerts > 0 ) {
                vertices.resize(numVerts);
            }
        } else if ( strcmp( reader.currentToken(), "numtris" ) == 0 ) {
            u32 numTris = static_cast<u32>(reader.readFloat());

            if ( numTris > 0 ) {
                triangles.resize(numTris);
            }
        } else if ( strcmp( reader.currentToken(), "numweights" ) == 0 ) {
            u32 numWeights = static_cast<u32>(reader.readFloat());

            if ( numWeights > 0 ) {
                weights.resize(numWeights);
            }
        } else if ( strcmp( reader.currentToken(), "vert" ) == 0 ) {
            //vert %d ( %f %f ) %d %d
            u32 vertIndex = static_cast<u32>(reader.readFloat());
            reader.readToken();//(
            Md5Vertex& vertex = vertices[vertIndex];
            vertex.st.x = reader.readFloat();
            vertex.st.y = reader.readFloat();
            reader.readToken();//)
            vertex.start = static_cast<u32>(reader.readFloat());
            vertex.count = static_cast<u32>(reader.readFloat());
        } else if ( strcmp( reader.currentToken(), "tri" ) == 0 ) {
            // tri %d %d %d %d
            u32 triangleIndex = static_cast<u32>(reader.readFloat());
            Md5Triangle& triangle = triangles[triangleIndex];
            triangle.index[0] = static_cast<u32>(reader.readFloat());
            triangle.index[1] = static_cast<u32>(reader.readFloat());
            triangle.index[2] = static_cast<u32>(reader.readFloat());
        } else if ( strcmp( reader.currentToken(), "weight" ) == 0 ) {
            //weight %d %d %f ( %f %f %f )
            u32 weightIndex = static_cast<u32>(reader.readFloat());
            Md5Weight& weight = weights[weightIndex];
            weight.joint = static_cast<u32>(reader.readFloat());
            weight.bias  = reader.readFloat();
            reader.readToken(); //(
            readVector( reader, weight.pos );
            reader.readToken(); //)
        }
    }
}

}
}
