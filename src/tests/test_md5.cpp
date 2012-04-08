/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 *              Victor Sukochev  <sukochevvv@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "renderer/md5mesh.h"

using base::opengl::Entity;

TEST(mesh, md5_load)
{
    Entity* entity = Entity::Load("hellknight.md5mesh");
    EXPECT_TRUE(entity != NULL);
    delete entity;
}
