/**
 * @file    base/stream.h
 * @brief   stream object (read/write operations over file, memory buffer)
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/types.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace base {

template<class T>
class Stream {
protected:
    u32 position_;

public:
    Stream() : position_(0) {}
    virtual ~Stream() {}

    void read(u8* dest, u32 size) {
        T* pthis = static_cast<T*>(this);
        pthis->read_impl(dest, size, position_);
        position_ += size;
    }

    template <typename TT>
    TT read_type() {
        TT ret;
        read(reinterpret_cast<u8*>(&ret), sizeof(TT));
        return ret;
    }

    template <typename TT>
    void read_template(TT& value) {
        value = read_type<TT>();
    }

    void write(const u8* source, u32 size) {
        T* pthis = static_cast<T*>(this);
        pthis->write_impl(source, size, position_);
        position_ += size;
    }

    template <typename TT>   
    void write_template(const TT& value) {
        write(reinterpret_cast<const u8*>(&value), sizeof(TT));
    }

    u32 position() const { return position_; }
    void set_position(u32 position) { position_ = position; }
};

class FileBinary : public Stream<FileBinary> {
protected:
    std::fstream file_;
public:
    FileBinary(const std::string& filename);
    virtual ~FileBinary();
    u32 size();
    std::string get_line();

    void read_impl(u8* dest, u32 size, u32 position);
    void write_impl(const u8* source, u32 size, u32 position);
protected:
    std::istream& safeGetline(std::istream& is, std::string& t, u32& read_count);
};

class FileText {
protected:
    std::fstream file_;
public:
    FileText(const std::string& filename);
    virtual ~FileText();
    std::vector<std::string> read_lines();
    std::string read_all();
    u32 size();
};

}