#pragma once

#include "base/types.h"
#include <assert.h>

class Statistics
{
private:
    u32 polygons_;
    u32 drawcalls_;
    u32 program_switches_;
    u32 texture_switches_;

public:
    Statistics() {
        reset();
    }

    void reset() {
        polygons_ = 0;
        drawcalls_ = 0;
        texture_switches_ = 0;
        program_switches_ = 0;
    }

    u32 polygons() const {
        return polygons_;
    }
    
    u32 drawcalls() const {
        return drawcalls_;
    }

    u32 texture_switches() const {
        return texture_switches_;
    }

    u32 program_switches() const {
        return program_switches_;
    }

    void add_polygons( u32 p ) {
        polygons_ += p;
        drawcalls_++;
    }

    void inc_texture_switches() {
        texture_switches_++;
    }

    void inc_program_switches() {
        program_switches_++;
    }
};

template<typename Type>
class Singleton
{
protected:
    static Type* instance_;
public:
    static void init() {
        assert( !hasInstance() );
        instance_ = new Type;
    }
    static void shutdown() {
        assert( hasInstance() );
        delete instance_;
        instance_ = NULL;
    }
    static bool hasInstance() {
        return instance_ != NULL;
    }
    static Type* instance() {
        assert( hasInstance() );
        return instance_;
    }
};
template<typename Type> Type* Singleton<Type>::instance_ = NULL;

class Stats : public Singleton<Statistics>
{
public:
    static void reset_frame() {
        instance()->reset();
    }
    static u32 polygons() {
        return instance()->polygons();
    }
    static u32 drawcalls() {
        return instance()->drawcalls();
    }
    static void inc_program_switches() {
        instance()->inc_program_switches();
    }
    static void inc_texture_switches() {
        instance()->inc_texture_switches();
    }
    static void add_polygons( u32 p ) {
        instance()->add_polygons( p );
    }
private:
    Stats();
    DISALLOW_COPY_AND_ASSIGN(Stats);
};
