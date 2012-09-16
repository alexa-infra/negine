/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "renderer/md3mesh.h"

using base::resource::Md3Model;

TEST( mesh, md3_load_by_chunks )
{
    Md3Model model;
    model.Load( "european_fnt_v2.md3" );
    EXPECT_TRUE( true );
}

TEST( mesh, md3_load_by_memory )
{
    //std::vector<Mesh*> mesh_;
    //mesh_ = load_md3_se("european_fnt_v2.md3");
    //for (u32 i=0; i<mesh_.size(); i++)
    //    delete mesh_[i];
    //EXPECT_TRUE(true);
}