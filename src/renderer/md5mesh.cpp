#include "md5mesh.h"
#include "base/lexer.h"

namespace base {
namespace opengl {

void readVector(Lexer& reader, math::Vector3& vec)
{
    vec.x = reader.ReadFloat();
    vec.y = reader.ReadFloat();
    vec.z = reader.ReadFloat();
}

void readQuat(Lexer& reader, math::Quat& q)
{
    q.x = reader.ReadFloat();
    q.y = reader.ReadFloat();
    q.z = reader.ReadFloat();
}

Entity* Entity::Load(const string& filename)
{
    Lexer reader(filename);

    Md5Model model;
    while(reader.HasMoreData())
    {
        reader.ReadToken();

        if (reader.CurrentToken() == "MD5Version")
        {
            u32 version = (u32) reader.ReadFloat();
            if ( version!=10 )
            {
                return NULL;
            }
        }
        else if (reader.CurrentToken() == "numJoints")
        {
            model.num_joints = (u32) reader.ReadFloat();
            if (model.num_joints > 0)
            {
                model.baseSkel = new Md5Joint[model.num_joints];
            } 
        }
        else if (reader.CurrentToken() == "numMeshes")
        {
            model.num_meshes = (u32) reader.ReadFloat();
            if (model.num_meshes > 0)
            {
                model.meshes = new Md5Mesh[model.num_meshes];
            } 
        }
        else if (reader.CurrentToken() == "joints")
        {
            reader.ReadToken(); // {

            for (i32 i = 0; i < model.num_joints; ++i)
            {
                Md5Joint& joint = model.baseSkel[i];
                
                //"name parent ( pos.x pos.y pos.z ) ( q.x q.y q.z )"
                joint.name = reader.ReadToken();
                joint.parent = (i32)reader.ReadFloat();

                reader.ReadToken(); // (
                readVector(reader, joint.pos);
                reader.ReadToken(); // )
                
                reader.ReadToken(); // (
                readQuat(reader, joint.orient);
                reader.ReadToken(); // )
                
                joint.orient.ComputeW();
            }

            reader.ReadToken(); // }
        }
        else if (reader.CurrentToken() == "mesh")
        {
            reader.ReadToken(); // {

            reader.ReadToken(); // }
        }
    }

    return NULL;
}

}
}