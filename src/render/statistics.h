#pragma once

#include "base/types.h"
#include "base/singleton.h"

namespace base {

class Statistics
{
private:
    u32 polygons_;
    u32 drawcalls_;
    u32 program_switches_;
    u32 texture_switches_;

public:
    Statistics();

    void reset();
    u32 polygons() const;
    u32 drawcalls() const;
    u32 texture_switches() const;
    u32 program_switches() const;
    void add_polygons( u32 p );
    void inc_texture_switches();
    void inc_program_switches();
};

class Stats : public base::Singleton<Statistics>
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

} // namespace base
