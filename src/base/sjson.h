#pragma once

#include "base/types.h"
#include "base/debug.h"
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace base {
namespace sjson {

    enum ValueType {
        typeTrue, typeFalse, typeInt, typeFloat, typeString, typeArray, typeDict, typeNull
    };

    struct Variant {
    public:
        typedef std::map<std::string, Variant> Map;
        typedef std::vector<Variant> Array;

    private:
        ValueType type;

        std::shared_ptr<i64>            intVal;
        std::shared_ptr<f64>            floatVal;
        std::shared_ptr<std::string>    strVal;
        std::shared_ptr<Map>            mapVal;
        std::shared_ptr<Array>          arrVal;
    public:
        Variant();
        explicit Variant(ValueType v);
        explicit Variant(bool val);
        explicit Variant(i64 val);
        explicit Variant(f64 val);
        explicit Variant(const std::string& val);

        bool isBool() const;
        bool isInt() const;
        bool isFloat() const;
        bool isString() const;
        bool isMap() const;
        bool isArray() const;
        bool isNull() const;

        bool asBool() const;
        std::string& asString() const;

        template<typename T>
        T asInt() const {
            ASSERT(isInt());
            return static_cast<T>(*intVal);
        }
        template<typename T>
        T asFloat() const {
            ASSERT(isFloat());
            return static_cast<T>(*floatVal);
        }

        Array& asArray() const;
        Map& asMap() const;

        bool hasMember(const std::string& name) const;
        Variant& operator[](const std::string& name);
        const Variant& operator[](const std::string& name) const;
        Variant& operator[](const int idx);
        const Variant& operator[](const int idx) const;
        size_t size() const;
    };

    bool parse(const std::string& json, Variant& obj, bool simplified = true);
    std::string write(const Variant& v, bool simplified = true);
    
    bool readFile(const std::string& filename, Variant& obj, bool simplified = true);
    void writeFile(const std::string& filename, Variant& obj, bool simplified = true);

} // namespace sjson
} // namespace base
