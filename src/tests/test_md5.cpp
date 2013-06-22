/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "render/md5mesh.h"

using base::resource::Entity;

TEST( mesh, md5_load )
{
    Entity* entity = Entity::Load( "hellknight.md5mesh" );
    EXPECT_TRUE( entity != NULL );
    EXPECT_EQ( entity->object.md5Model.baseSkel.size(), 110 );
    EXPECT_EQ( entity->object.md5Model.meshes.size(), 2 );
    EXPECT_EQ( entity->object.md5Model.meshes[0].vertices.size(), 1656 );
    delete entity;
}

TEST ( mesh, GenerateGPUVertices )
{
    Entity* entity = Entity::Load( "hellknight.md5mesh" );
    delete entity;
}
