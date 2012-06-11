/**
 * \file
 * \brief       stream object (read/write operations over file, memory buffer)
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
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
    TT read_type_nomove() {
        TT ret;
        T* pthis = static_cast<T*>(this);
        pthis->read_impl(reinterpret_cast<u8*>(&ret), sizeof(TT), position_);
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

protected:
    void read_impl(u8* dest, u32 size, u32 position);
    void write_impl(const u8* source, u32 size, u32 position);

    std::istream& safeGetline(std::istream& is, std::string& t, u32& read_count);
    friend class Stream<FileBinary>;
};

class FileText {
protected:
    std::fstream file_;
    std::streambuf* sb_;
    u32 size_;
    u32 position_;
public:
    FileText(const std::string& filename);
    virtual ~FileText();
    std::vector<std::string> read_lines();
    std::string read_all();

    char current_char();
    char bump_char();

    u32 position() const { return position_; }
    u32 size() const { return size_; }
};

bool file_exists(const std::string& name);

} // namespace base
