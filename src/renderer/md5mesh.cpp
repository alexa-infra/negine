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
    u32 currentMeshIndex = 0;

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
            Md5Mesh &mesh  = model.meshes[currentMeshIndex];
            while (reader.CurrentToken() != "}" && reader.HasMoreData())
            {
                reader.ReadToken();
                if (reader.CurrentToken() == "shader")
                {
                    string materialName = reader.ReadToken();
                }
                else if (reader.CurrentToken() == "bumpShader")
                {
                    string materialName = reader.ReadToken();
                }
                else if (reader.CurrentToken() == "numverts")
                {                  
                    mesh.num_verts = (u32) reader.ReadFloat();
                    if ( mesh.num_verts > 0)
                    {
                        mesh.vertices = new Md5Vertex [mesh.num_verts];
                    }
                }
                else if (reader.CurrentToken() == "numtris")
                {
                     mesh.num_tris = (u32) reader.ReadFloat();
                    if ( mesh.num_tris > 0)
                    {
                        mesh.triangles = new Md5Triangle [mesh.num_tris];
                    }                    
                }
                else if (reader.CurrentToken() == "numweights")
                {
                    mesh.num_weights = (u32) reader.ReadFloat();
                    if ( mesh.num_weights > 0)
                    {
                        mesh.weights = new Md5Weight [mesh.num_weights];
                    }
                }
                else if (reader.CurrentToken() == "vert")
                {
                    //vert %d ( %f %f ) %d %d
                    u32 vertIndex = (u32) reader.ReadFloat();
                    reader.ReadToken();//(

                    Md5Vertex &vertex = mesh.vertices[vertIndex];
                    vertex.st.x = reader.ReadFloat();
                    vertex.st.y = reader.ReadFloat();

                    reader.ReadToken();//)

                    vertex.start = (u32) reader.ReadFloat();
                    vertex.count = (u32) reader.ReadFloat();

                }
                else if (reader.CurrentToken() == "tri")
                {
                    // tri %d %d %d %d
                    u32 triangleIndex = (u32) reader.ReadFloat();
                    Md5Triangle &triangle = mesh.triangles[triangleIndex];

                    triangle.index[0] = (u32) reader.ReadFloat();
                    triangle.index[1] = (u32) reader.ReadFloat();
                    triangle.index[2] = (u32) reader.ReadFloat();
                }
                else if (reader.CurrentToken() == "weight")
                {
                    //weight %d %d %f ( %f %f %f )
                    u32 weightIndex = (u32) reader.ReadFloat();
                    Md5Weight &weight = mesh.weights[weightIndex];

                    weight.joint = (u32) reader.ReadFloat();
                    weight.bias  = reader.ReadFloat();

                    reader.ReadToken(); //(
                    readVector(reader, weight.pos);
                    reader.ReadToken(); //)

                }
            }
            currentMeshIndex++;
        }
    }

    return NULL;
}

}
}