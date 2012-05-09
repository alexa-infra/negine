#include "md5mesh.h"
#include "base/lexer.h"
#include <string.h>

namespace base {
namespace opengl {

math::Vector3 Md5Joint::translate(const math::Vector3& v) const
{
    return orient.RotatePoint(v) + pos;
}

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

    Entity* entity = new Entity;

    Md5Model& model = entity->object.md5Model;
    while(reader.HasMoreData())
    {
        reader.ReadToken();

        if (strcmp(reader.CurrentToken(), "MD5Version") == 0)
        {
            u32 version = (u32) reader.ReadFloat();
            if ( version!=10 )
            {
                return NULL;
            }
        }
        else if (strcmp(reader.CurrentToken(), "numJoints") == 0)
        {
            model.num_joints = (u32) reader.ReadFloat();
            if (model.num_joints > 0)
            {
                model.baseSkel = new Md5Joint[model.num_joints];
            } 
        }
        else if (strcmp(reader.CurrentToken(), "numMeshes") == 0)
        {
            model.num_meshes = (u32) reader.ReadFloat();
            if (model.num_meshes > 0)
            {
                model.meshes = new Md5Mesh[model.num_meshes];
            } 
        }
        else if (strcmp(reader.CurrentToken(), "joints") == 0)
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
        else if (strcmp(reader.CurrentToken(), "mesh") == 0)
        {
            reader.ReadToken(); // {
            Md5Mesh &mesh  = model.meshes[currentMeshIndex];
            while (strcmp(reader.CurrentToken(), "}") != 0 && reader.HasMoreData())
            {
                reader.ReadToken();
                if (strcmp(reader.CurrentToken(), "shader") == 0)
                {
                    mesh.shader = reader.ReadToken();
                }
                else if (strcmp(reader.CurrentToken(), "numverts") == 0)
                {                  
                    mesh.num_verts = (u32) reader.ReadFloat();
                    if ( mesh.num_verts > 0)
                    {
                        mesh.vertices    = new Md5Vertex [mesh.num_verts];
                        mesh.vertexArray = new Vertex[mesh.num_verts];
                    }
                }
                else if (strcmp(reader.CurrentToken(), "numtris") == 0)
                {
                     mesh.num_tris = (u32) reader.ReadFloat();
                    if ( mesh.num_tris > 0)
                    {
                        mesh.vertexIndices = new Face[mesh.num_tris];
                        mesh.triangles     = new Md5Triangle [mesh.num_tris];
                    }                    
                }
                else if (strcmp(reader.CurrentToken(), "numweights") == 0)
                {
                    mesh.num_weights = (u32) reader.ReadFloat();
                    if ( mesh.num_weights > 0)
                    {
                        mesh.weights = new Md5Weight [mesh.num_weights];
                    }
                }
                else if (strcmp(reader.CurrentToken(), "vert") == 0)
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
                else if (strcmp(reader.CurrentToken(), "tri") == 0)
                {
                    // tri %d %d %d %d
                    u32 triangleIndex = (u32) reader.ReadFloat();
                    Md5Triangle &triangle = mesh.triangles[triangleIndex];

                    triangle.index[0] = (u32) reader.ReadFloat();
                    triangle.index[1] = (u32) reader.ReadFloat();
                    triangle.index[2] = (u32) reader.ReadFloat();
                }
                else if (strcmp(reader.CurrentToken(), "weight") == 0)
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

    return entity;
}

void Entity::GenerateGPUVertices(Md5Mesh &mesh, const Md5Joint* skeleton)
{
    
    Vertex* currentVertex = mesh.vertexArray;
    for (int i = 0; i < mesh.num_verts; ++i)
    {
        currentVertex->tex = mesh.vertices[i].st;

        currentVertex->pos.set(0.0f);
        currentVertex->n.set(0.0f);
        currentVertex->tangent.set(0.0f);
       
        for (int j = 0; j < mesh.vertices[i].count; j++)
        {
            const Md5Weight& weight = mesh.weights[mesh.vertices[i].start + j];
            const Md5Joint& joint = skeleton[weight.joint];
            
            currentVertex->pos += joint.translate(weight.pos) * weight.bias;
            currentVertex->n += joint.orient.RotatePoint(weight.normal);
            currentVertex->tangent += joint.orient.RotatePoint(weight.tangent);
        }

        currentVertex->n.Normalize();
        currentVertex->tangent.Normalize();

        currentVertex++;
    }

    
    for (int i = 0; i<mesh.num_tris; i++)
    {
        for (int j=0; j<3; j++)
        {
            mesh.vertexIndices[i].index[j] = (u16)mesh.triangles[i].index[j];
        }
    }

    GenerateLightningInfo(mesh);
}

void Entity::GenerateLightningInfo(Md5Mesh &mesh)
{
    for ( u32 i = 0; i < mesh.num_tris; i++)
    {
        f32 d0[5], d1[5];
        Vertex* a;
        Vertex* b;
        Vertex* c;

        math::Vector3 temp, normal, tangents[2];

        a = mesh.vertexArray + mesh.triangles[i].index[0];
        b = mesh.vertexArray + mesh.triangles[i].index[1];
        c = mesh.vertexArray + mesh.triangles[i].index[2];

        d0[0] = b->pos.x - a->pos.x;
        d0[1] = b->pos.y - a->pos.y;
        d0[2] = b->pos.z - a->pos.z;
        d0[3] = b->tex.x - a->tex.x;
        d0[4] = b->tex.y - a->tex.y;

        d1[0] = c->pos.x - a->pos.x;
        d1[1] = c->pos.y - a->pos.y;
        d1[2] = c->pos.z - a->pos.z;
        d1[3] = c->tex.x - a->tex.x;
        d1[4] = c->tex.y - a->tex.y;

        normal = Vector3(
            d1[1] * d0[2] - d1[2] * d0[1],
            d1[2] * d0[0] - d1[0] * d0[2],
            d1[0] * d0[1] - d1[1] * d0[0]
        );
        normal.Normalize();

        tangents[0] = Vector3(
            d0[0] * d1[4] - d0[4] * d1[0],
            d0[1] * d1[4] - d0[4] * d1[1],
            d0[2] * d1[4] - d0[4] * d1[2]
        );
        tangents[0].Normalize();

        tangents[1] = Vector3(
            d0[3] * d1[0] - d0[0] * d1[3],
            d0[3] * d1[1] - d0[1] * d1[3],
            d0[3] * d1[2] - d0[2] * d1[3]
        );
        tangents[1].Normalize();

        for (int j=0;j<3; j++) {
            Vertex* v = mesh.vertexArray + mesh.triangles[i].index[j];
            v->n += normal;
            v->tangent  += tangents[0];
            v->binormal += tangents[1];
        }
    }
    for (u32 i=0;i<mesh.num_verts;i++) 
    {
        mesh.vertexArray[i].n.Normalize();
        f32 d = Dot(mesh.vertexArray[i].tangent, mesh.vertexArray[i].n);
        mesh.vertexArray[i].tangent = mesh.vertexArray[i].tangent - mesh.vertexArray[i].n * d;
        mesh.vertexArray[i].tangent.Normalize();
        d = Dot(mesh.vertexArray[i].binormal, mesh.vertexArray[i].n);
        mesh.vertexArray[i].binormal = mesh.vertexArray[i].binormal - mesh.vertexArray[i].n * d;
        mesh.vertexArray[i].binormal.Normalize();
    }
}

}
}
