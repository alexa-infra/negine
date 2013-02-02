#include "statistics.h"

namespace base {

Statistics::Statistics() {
    reset();
}

void Statistics::reset() {
    polygons_ = 0;
    drawcalls_ = 0;
    texture_switches_ = 0;
    program_switches_ = 0;
}

u32 Statistics::polygons() const {
    return polygons_;
}

u32 Statistics::drawcalls() const {
    return drawcalls_;
}

u32 Statistics::texture_switches() const {
    return texture_switches_;
}

u32 Statistics::program_switches() const {
    return program_switches_;
}

void Statistics::add_polygons( u32 p ) {
    polygons_ += p;
    drawcalls_++;
}

void Statistics::inc_texture_switches() {
    texture_switches_++;
}

void Statistics::inc_program_switches() {
    program_switches_++;
}

} // namespace base
