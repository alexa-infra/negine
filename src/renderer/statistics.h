#pragma once

#include "base/types.h"
#include <assert.h>

class Statistics
{
private:
    u32 polygons_;
    u32 drawcalls_;

public:
    Statistics() {
        reset();
    }

    void reset() {
        polygons_ = 0;
        drawcalls_ = 0;
    }

    u32 polygons() const {
        return polygons_;
    }
    
    u32 drawcalls() const {
        return drawcalls_;
    }

    void add_polygons( u32 p ) {
        polygons_ += p;
        drawcalls_++;
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

    static void add_polygons( u32 p ) {
        instance()->add_polygons( p );
    }
private:
    Stats();
    DISALLOW_COPY_AND_ASSIGN(Stats);
};
