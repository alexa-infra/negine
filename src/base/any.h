#pragma once

#include <type_traits>
#include <algorithm>

namespace base {

//! Any type holder
//! Unsafe version of boost.any class
//! https://github.com/ryppl/boost-svn/blob/master/boost/any.hpp
class any
{
private:
    class placeholder
    {
    public:
        virtual ~placeholder() {}
        virtual placeholder* clone() = 0;
    };

    template<typename ValueType>
    class holder : public placeholder
    {
    public:
        holder(const ValueType& value) : held(value) {}
        placeholder* clone() { return new holder(held); }
        ValueType held;
    };
public:
    template<typename ValueType>
    friend ValueType* any_cast(any*);

    any() : content(nullptr) {}

    template<typename ValueType>
    any(const ValueType& value) : content(new holder<ValueType>(value)) {}

    ~any() { delete content; }

    any& swap(any& rhs) { std::swap(content, rhs.content); return *this; }
    any& operator=(any rhs) { rhs.swap(*this); return *this; }
    template<typename ValueType>
    any& operator=(const ValueType& value) { any(value).swap(*this); return *this; }

    bool empty() const { return !content; }

private:
    placeholder* content;
};

template<typename ValueType>
inline ValueType* any_cast(any* operand) {
    return &static_cast<any::holder<ValueType>*>(operand->content)->held;
}

template<typename ValueType>
inline const ValueType* any_cast(const any* operand) {
    return any_cast<ValueType>(const_cast<any*>(operand));
}

template<typename ValueType>
ValueType any_cast(any& operand) {
    typedef typename std::remove_reference<ValueType>::type nonref;
    nonref* result = any_cast<nonref>(&operand);
    return *result;
}

template<typename ValueType>
inline ValueType any_cast(const any& operand) {
    typedef typename std::remove_reference<ValueType>::type nonref;
    return any_cast<const nonref&>(const_cast<any&>(operand));
}

} // namespace base
