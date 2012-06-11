/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "base/stream.h"

namespace base {

FileBinary::FileBinary(const std::string& filename) {
    file_.open(filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
}

FileBinary::~FileBinary() {
    if (file_.is_open()) file_.close();
}

u32 FileBinary::size() {
    file_.seekg(0, std::ios::end);
    return static_cast<u32>(file_.tellg());
}

std::string FileBinary::get_line() {
    file_.seekg(position_);
    std::string ret;
    //u32 len = 0;
    //safeGetline(file_, ret, len);
    std::getline(file_, ret);
    position_ = static_cast<u32>(file_.tellg());
    return ret;
}

void FileBinary::read_impl(u8* dest, u32 size, u32 position) {
    file_.seekg(position);
    file_.read(reinterpret_cast<char*>(dest), size);
}

void FileBinary::write_impl(const u8* source, u32 size, u32 position) {
    file_.seekp(position);
    file_.write(reinterpret_cast<const char*>(source), size);
}

std::istream& FileBinary::safeGetline(std::istream& is, std::string& t, u32& read_count)
{
    t.clear();
//    std::istream::sentry se(is);
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

FileText::FileText(const std::string& filename)
    : sb_(NULL)
    , size_(0)
    , position_(0)
{
    file_.open(filename.c_str(), std::ios::binary | std::ios::in);
    if (file_.good())
    {
        file_.seekg(0, std::ios::end);
        size_ = static_cast<u32>(file_.tellg());
        file_.seekg(0, std::ios::beg);
        sb_ = file_.rdbuf();
    }
}

FileText::~FileText() {
    if (file_.is_open()) file_.close();
}

std::vector<std::string> FileText::read_lines() {
    file_.seekg(0, std::ios::beg);
    std::vector<std::string> ret;
    std::string str;
    while(std::getline(file_, str)) {
        ret.push_back(str);
    }
    return ret;
}

std::string FileText::read_all() {
    u32 s = size();
    
    std::string ret;
    ret.resize(s);
    char* buf = const_cast<char*>(ret.c_str());

    file_.seekg(0, std::ios::beg);
    file_.read(buf, s);

    return ret;
}

char FileText::current_char() {
    return static_cast<char>(sb_->sgetc());
}

char FileText::bump_char() {
    position_++;
    return static_cast<char>(sb_->sbumpc());
}

bool file_exists(const std::string& name)
{
    std::ifstream f(name.c_str());;
    return f;
}

} // namespace base
