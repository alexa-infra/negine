/**
 * \file
 * \brief       stream object (read/write operations over file, memory buffer)
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include <fstream>
#include <vector>
#include <string>

namespace base
{

//! Template base class for binary stream operations
//! It stores position in stream (read/write)
template<class T>
class BinaryStreamBase
{
public:
    BinaryStreamBase() : position_( 0 ) {}
    virtual ~BinaryStreamBase() {}

    //! Read raw bytes from stream
    void readRaw( u8* dest, u32 size ) {
        T* pthis = static_cast<T*>( this );
        pthis->readImpl( dest, size, position_ );
        position_ += size;
    }

    //! Read raw object from stream
    template <typename TT>
    TT readType() {
        TT ret;
        u8* ptr = reinterpret_cast<u8*>( &ret );
        readRaw( ptr, sizeof( ret ) );
        return ret;
    }

    //! Read raw object from stream (reference version)
    template <typename TT>
    void read( TT& value ) {
        value = readType<TT>();
    }

    //! Write raw bytes to stream
    void writeRaw( const u8* source, u32 size ) {
        T* pthis = static_cast<T*>( this );
        pthis->writeImpl( source, size, position_ );
        position_ += size;
    }

    //! 
    template <typename TT>
    void write( const TT& value ) {
        const u8* ptr = reinterpret_cast<const u8*>( &value );
        writeRaw( ptr, sizeof( value ) );
    }

    u32 position() const {
        return position_;
    }

    void setPosition( u32 position ) {
        position_ = position;
    }
protected:
    u32 position_;
};

//! Binary file wrapper for input/output
class FileBinary : public BinaryStreamBase<FileBinary>
{
    friend class BinaryStreamBase<FileBinary>;

public:
    FileBinary( const std::string& filename );
    virtual ~FileBinary();
    u32 size();

protected:
    void readImpl( u8* dest, u32 size, u32 position );
    void writeImpl( const u8* source, u32 size, u32 position );

protected:
    std::fstream file_;
};

//! Text file wrapper for not-formatted reading 
class FileText
{
public:
    FileText( const std::string& filename );
    virtual ~FileText();
    
    //! Reads all lines
    std::vector<std::string> readLines();
    
    //! Reads all text to string
    std::string readAll();

    //! Reads next line
    std::string readLine();

    //! Gets the character at the current position
    char currentChar();

    //! Gets the character at the current position, and advances current position 
    char bumpChar();

    u32 position() const {
        return position_;
    }

    u32 size() const {
        return size_;
    }

protected:
    std::fstream file_;
    std::streambuf* sb_;
    u32 size_;
    u32 position_;
};

} // namespace base
