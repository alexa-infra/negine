#pragma once

#include "base/types.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace base {

class Stream {
protected:
    u32 position_;

public:
    Stream() : position_(0) {}
    virtual ~Stream() {}

    void read(u8* dest, u32 size) {
        read_impl(dest, size, position_);
        position_ += size;
    }

    template <typename T>
    T read_type() {
        T ret;
        read(reinterpret_cast<u8*>(&ret), sizeof(T));
        return ret;
    }

    template <typename T>
    void read_template(T& value) {
        value = read_type<T>();
    }

    void write(const u8* source, u32 size) {
        write_impl(source, size, position_);
        position_ += size;
    }

    template <typename T>   
    void write_template(const T& value) {
        write(reinterpret_cast<const u8*>(&value), sizeof(T));
    }

    u32 position() const { return position_; }
    void set_position(u32 position) { position_ = position; }

protected:
    virtual void read_impl(u8* dest, u32 size, u32 position) = 0;
    virtual void write_impl(const u8* source, u32 size, u32 position) = 0;
};

class FileBinary : public Stream {
protected:
    std::fstream file_;
public:
    FileBinary(const std::string& filename) {
        file_.open(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    }
    virtual ~FileBinary() {
        if (file_.is_open()) file_.close();
    }
    u32 size() {
        file_.seekg(0, std::ios::end);
        return file_.tellg();
    }
    std::string get_line() {
        file_.seekg(position_);
        std::string ret;
        //u32 len = 0;
        //safeGetline(file_, ret, len);
        std::getline(file_, ret);
        position_ = file_.tellg();
        return ret;
    }
protected:
    void read_impl(u8* dest, u32 size, u32 position) {
        file_.seekg(position);
        file_.read(reinterpret_cast<char*>(dest), size);
    }
    void write_impl(const u8* source, u32 size, u32 position) {
        file_.seekp(position);
        file_.write(reinterpret_cast<const char*>(source), size);
    }
    std::istream& safeGetline(std::istream& is, std::string& t, u32& read_count)
    {
        t.clear();
        std::istream::sentry se(is);
        std::streambuf* sb = is.rdbuf();

        for(read_count=1;;read_count++) {
            int c = sb->sbumpc();
            switch (c) {
            case '\r':
                c = sb->sgetc();
                if(c == '\n') {
                    sb->sbumpc();
                    read_count++;
                }
                return is;
            case '\n':
            case EOF:
                return is;
            default:
                t += (char)c;
            }
        }
}
};

class FileText {
protected:
    std::fstream file_;
public:
    FileText(const std::string& filename) {
        file_.open(filename.c_str(), std::ios::in | std::ios::out);
    }
    virtual ~FileText() {
        if (file_.is_open()) file_.close();
    }
    std::vector<std::string> read_lines() {
        file_.seekg(0, std::ios::beg);
        std::vector<std::string> ret;
        std::string str;
        while(std::getline(file_, str)) {
            ret.push_back(str);
        }
        return ret;
    }
    std::string read_all() {
        u32 s = size();
        
        std::string ret;
        ret.resize(s);
        char* buf = const_cast<char*>(ret.c_str());

        file_.seekg(0, std::ios::beg);
        file_.read(buf, s);

        return ret;
    }
    u32 size() {
        file_.seekg(0, std::ios::end);
        return file_.tellg();
    }
};

}