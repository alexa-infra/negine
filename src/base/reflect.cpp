#include "base/reflect.h"

namespace base
{

void oarchive::reflect(const std::string& name, bool& object) {
    pushNode(name, sjson::Variant(object));
}

void oarchive::reflect(const std::string& name, std::string& object) {
    pushNode(name, sjson::Variant(object));
}

void oarchive::reflect(const std::string& name, i32& object) {
    pushNode(name, sjson::Variant(static_cast<i64>(object)));
}

void oarchive::reflect(const std::string& name, u32& object) {
    pushNode(name, sjson::Variant(static_cast<i64>(object)));
}

void oarchive::reflect(const std::string& name, f32& object) {
    pushNode(name, sjson::Variant(static_cast<f64>(object)));
}

void oarchive::pushNode(const std::string& name, const sjson::Variant& node)
{
    sjson::Variant& current = currentNode();
    if(current.isMap())
        current[name] = node;
    else if (current.isArray())
        current.asArray().push_back(node);
}

sjson::Variant& oarchive::currentNode() {
    return *(nodes_.top());
}

void iarchive::reflect(const std::string& name, bool& object) {
    object = currentNode(name).asBool();
}

void iarchive::reflect(const std::string& name, std::string& object) {
    object = currentNode(name).asString();
}

void iarchive::reflect(const std::string& name, i32& object) {
    object = currentNode(name).asInt<i32>();
}

void iarchive::reflect(const std::string& name, u32& object) {
    object = currentNode(name).asInt<u32>();
}

void iarchive::reflect(const std::string& name, f32& object) {
    object = currentNode(name).asFloat<f32>();
}

const sjson::Variant& iarchive::currentNode(const std::string& name) {
    if (name.length() == 0)
        return *nodes_.top();
    return nodes_.top()->operator[](name);
}

}
