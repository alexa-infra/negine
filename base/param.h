/**
 * @file    base/param.h
 * @brief   wrapper for parameter
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GLPv3) – see LICENSE file for details
 **/
#pragma once

#include "base/classtypes.h"

namespace base {

template<typename T> class generic_param;

class param {
public:
    template<typename T>
    const T& get() const {
        const generic_param<T>* pthis = static_cast<const generic_param<T>*>(this);
        return pthis->Value;
    }

    template<typename T>
    void set(const T& value) {
        generic_param<T>* pthis = static_cast<generic_param<T>*>(this);
        pthis->Value = value;
    }

    virtual Type get_type() const { return Types::Unknown; }
};

template<typename T>
class generic_param : public param {
public:
    T Value;
    Type get_type() const { return class_info<T>::type(); }
};

} // namespace base
