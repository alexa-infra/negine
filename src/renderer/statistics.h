#pragma once

#include "base/types.h"
#include <assert.h>

class Stats
{
    u32 polygons_;
    u32 drawcalls_;
    static Stats* instance_;

    Stats() {
        reset();
    }

    void reset() {
        polygons_ = 0;
        drawcalls_ = 0;
    }

public:
    static void init() {
        assert( instance_ == NULL );
        instance_ = new Stats;
    }
    static void shutdown() {
        assert( instance_ != NULL );
        delete instance_;
        instance_ = NULL;
    }
    static void reset_frame() {
        instance_->reset();
    }
    static u32 polygons() {
        return instance_->polygons_;
    }
    static u32 drawcalls() {
        return instance_->drawcalls_;
    }

    static void add_polygons( u32 p ) {
        instance_->polygons_ += p;
        instance_->drawcalls_++;
    }
};
