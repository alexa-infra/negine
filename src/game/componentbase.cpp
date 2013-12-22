#include "componentbase.h"

namespace base {
namespace game {

u32 ComponentBase::s_counter_ = 0;

CompType ComponentBase::registerType() {
    return ++s_counter_;
}

ComponentBase::ComponentBase()
    : parent_(nullptr)
{
}

ComponentBase::~ComponentBase()
{
}

} // namespace game
} // namespace base