#pragma once

#include "base/types.h"
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace base {
namespace sjson {

    enum ValueType {
        typeBool, typeInt, typeFloat, typeString, typeArray, typeDict, typeNull
    };

    struct Variant {
        ValueType type;

        typedef std::map<std::string, Variant> Map;
        typedef std::vector<Variant> Array;
        
        std::shared_ptr<bool>           boolVal;
        std::shared_ptr<i64>            intVal;
        std::shared_ptr<f64>            floatVal;
        std::shared_ptr<std::string>    strVal;
        std::shared_ptr<Map>            mapVal;
        std::shared_ptr<Array>          arrVal;

        Variant() : type(typeNull) {}
        explicit Variant(ValueType v);
        explicit Variant(bool val) : type(typeBool) {
            boolVal = std::make_shared<bool>(val);
        }
        explicit Variant(i64 val) : type(typeInt) {
            intVal = std::make_shared<i64>(val);
        }
        explicit Variant(f64 val) : type(typeFloat) {
            floatVal = std::make_shared<f64>(val);
        }
        explicit Variant(const std::string& val) : type(typeString) {
            strVal = std::make_shared<std::string>(val);
        }

        bool isBool() const {
            return type == typeBool && boolVal;
        }
        bool isInt() const {
            return type == typeInt && intVal;
        }
        bool isFloat() const {
            return type == typeFloat && floatVal;
        }
        bool isString() const {
            return type == typeString && strVal;
        }
        bool isMap() const {
            return type == typeDict && mapVal;
        }
        bool isArray() const {
            return type == typeArray && arrVal;
        }
        bool isNull() const {
            return type == typeNull;
        }

        bool& asBool() const {
            ASSERT(isBool());
            return *boolVal;
        }
        std::string& asString() const {
            ASSERT(isString());
            return *strVal;
        }
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
        Array& asArray() const {
            ASSERT(isArray());
            return *arrVal;
        }
        Map& asMap() const {
            ASSERT(isMap());
            return *mapVal;
        }

        bool hasMember(const std::string& name) const {
            return asMap().count(name) == 1;
        }
        Variant& operator[](const std::string& name) {
            return asMap()[name];
        }
        Variant& operator[](const int idx) {
            return asArray()[idx];
        }
        size_t size() const {
            ASSERT(isMap() || isArray());
            if (isMap()) return asMap().size();
            if (isArray()) return asArray().size();
            return 0;
        }
    };

    bool parse(const std::string& json, Variant& obj);
    std::string write(const Variant& v);

    bool parseJSON(const std::string& json, Variant& obj);
    std::string writeJSON(const Variant& v);

} // namespace sjson
} // namespace base
