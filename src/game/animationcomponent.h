#pragma once

#include "game/components.h"
#include "base/types.h"

class AnimationComponent : public Component
{
public:
	u32 frame_count_;
	u32 frame_rate_;
	u32 current_frame_;
	u32 next_frame_;
	f32 interp_phase_;

	static ComponentType type() { return ComponentTypes::Animation; }

	void onAttach() {}
	void onDetach() {}

	void update(f32 timediff) {
		f32 frame_time = 1.0f / (f32)frame_rate_;
		
		f32 intpart, fractpart;
		fractpart = modf(timediff / frame_time, &intpart);

		interp_phase_ = fractpart;
		current_frame_ = (current_frame_ + (u32)intpart) % frame_count_;
		next_frame_ = (current_frame_ + 1) % frame_count_;
	}
};