#include "gtest/gtest.h"
#include "base/reflect.h"

using namespace base;

struct Ggg
{
    std::string name;
    u32 weight;
    std::vector<std::string> bages;
    
    bool operator==(const Ggg& obj) const { return name == obj.name && weight == obj.weight && bages == obj.bages; }

    template<class archive>
    void reflect(archive& ar)
    {
        ar.reflect("name", name, std::string(""));
        ar.reflect("weight", weight, 0u);
        ar.vreflect("bages", bages);
    }
};

TEST(aaa, bb)
{
    Ggg g;
    g.name = "";
    g.weight = 30;
    g.bages.push_back("ein");
    g.bages.push_back("zwei");
    oarchive ar;
    sjson::Variant roo = ar.reflectObject(g);
    std::string ret = sjson::write(roo);
    iarchive un;
    Ggg a;
    un.reflectObject(roo, a);
    EXPECT_EQ(g, a);

    Ggg b;

    std::string res;
    oarchive::serialize(res, g);
    iarchive::deserialize(res, b);
    EXPECT_EQ(g, b);
    std::cout << ret << std::endl;
}

#include "base/path.h"

TEST(path, basic)
{
    std::string p("c:\\web\\do");
    EXPECT_EQ(path::normpath(p), std::string("c:/web/do"));

    EXPECT_EQ(path::toDir("c:/web/do"), std::string("c:/web/do/"));
    EXPECT_EQ(path::toDir("c:/web/do/"), std::string("c:/web/do/"));
    
    auto r1 = path::split("c:/web/do/file");
    EXPECT_EQ(std::get<0>(r1), std::string("c:/web/do"));
    EXPECT_EQ(std::get<1>(r1), std::string("file"));

    r1 = path::split("c:/web/do/file/");
    EXPECT_EQ(std::get<0>(r1), std::string("c:/web/do/file"));
    EXPECT_EQ(std::get<1>(r1), std::string(""));

    r1 = path::splitext("file.txt");
    EXPECT_EQ(std::get<0>(r1), std::string("file"));
    EXPECT_EQ(std::get<1>(r1), std::string(".txt"));

    r1 = path::splitext("filetxt");
    EXPECT_EQ(std::get<0>(r1), std::string("filetxt"));
    EXPECT_EQ(std::get<1>(r1), std::string(""));

    EXPECT_EQ(path::join("a", "b", "c"), std::string("a/b/c"));
    EXPECT_EQ(path::join("a", "b/a/v", "c"), std::string("a/b/a/v/c"));
}