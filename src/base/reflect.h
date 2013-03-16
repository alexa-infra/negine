#pragma once

#include "base/sjson.h"
#include <stack>

namespace base
{

class iarchive
{
public:
    template<typename T>
    void reflectObject(const sjson::Variant& root, T& object) {
        ASSERT(nodes_.size() == 0);
        nodes_.push(&root);
        object.reflect(*this);
        nodes_.pop();
    }

    void reflect(const std::string& name, bool& object);
    void reflect(const std::string& name, std::string& object);
    void reflect(const std::string& name, i32& object);
    void reflect(const std::string& name, u32& object);
    void reflect(const std::string& name, f32& object);

    template<typename T>
    void reflect(const std::string& name, T& object) {
        const sjson::Variant& current = currentNode(name);
        nodes_.push(&current);
        object.reflect(*this);
        nodes_.pop();
    }

    template<typename T>
    void reflect(const std::string& name, T& object, const T& def) {
        if (!nodes_.top()->hasMember(name)) {
            object = def;
            return;
        }
        reflect(name, object);
    }

    template<typename T>
    void vreflect(const std::string& name, std::vector<T>& object) {
        if (!nodes_.top()->hasMember(name)) {
            return;
        }
        const sjson::Variant& current = currentNode(name);
        object.resize(current.size());
        for(u32 i=0; i<current.size(); i++)
        {
            nodes_.push(&current[i]);
            reflect("", object[i]);
            nodes_.pop();
        }
    }

    template<typename T>
    static void deserialize(const std::string& json, T& obj)
    {
        sjson::Variant root;
        if (sjson::parse(json, root))
        {
            iarchive ar;
            ar.reflectObject(root, obj);
        }
    }
    
    template<typename T>
    static void deserializeFile(const std::string& filename, T& obj)
    {
        sjson::Variant root;
        if (sjson::readFile(filename, root))
        {
            iarchive ar;
            ar.reflectObject(root, obj);
        }
    }
private:
    const sjson::Variant& currentNode(const std::string& name);
    
    std::stack<const sjson::Variant*> nodes_;
};

class oarchive
{
public:
    template<typename T>
    sjson::Variant reflectObject(T& object) {
        ASSERT(nodes_.size() == 0);
        sjson::Variant root(sjson::ValueType::typeDict);
        nodes_.push(&root);
        object.reflect(*this);
        nodes_.pop();
        return root;
    }

    void reflect(const std::string& name, bool& object);
    void reflect(const std::string& name, std::string& object);
    void reflect(const std::string& name, i32& object);
    void reflect(const std::string& name, u32& object);
    void reflect(const std::string& name, f32& object);

    template<typename T>
    void reflect(const std::string& name, T& object) {
        sjson::Variant newNode(sjson::ValueType::typeDict);
        pushNode(name, newNode);
        nodes_.push(&newNode);
        object.reflect(*this);
        nodes_.pop();
    }

    template<typename T>
    void reflect(const std::string& name, T& object, const T& def) {
        if (object == def) return;
        reflect(name, object);
    }

    template<typename T>
    void vreflect(const std::string& name, std::vector<T>& object) {
        sjson::Variant newNode(sjson::ValueType::typeArray);
        pushNode(name, newNode);
        nodes_.push(&newNode);
        for(u32 i=0; i<object.size(); i++)
            reflect("", object[i]);
        nodes_.pop();
    }

    template<typename T>
    static void serialize(std::string& json, T& obj)
    {
        oarchive ar;
        sjson::Variant root = ar.reflectObject(obj);
        json = sjson::write(root);
    }
    
    template<typename T>
    static void serializeFile(const std::string& filename, T& obj)
    {
        oarchive ar;
        sjson::Variant root = ar.reflectObject(obj);
        sjson::writeFile(filename, root);
    }
private:
    void pushNode(const std::string& name, const sjson::Variant& node);

    sjson::Variant& currentNode();

    std::stack<sjson::Variant*> nodes_;
};

}
