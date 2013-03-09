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