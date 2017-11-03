// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "animation.hpp"

#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "resources/md5anim.hpp"

namespace tec {
	Animation::Animation(std::shared_ptr<MD5Anim> animation)
			: frame_count(0), animation_time(0.0f) {
		SetAnimationFile(animation);
		if (animation) {
			this->animation_name = animation_file->GetName();
		}
	}
	void Animation::UpdateAnimation(const double delta) {
		if (this->frame_count < 1) {
			return;
		}

		this->animation_time += delta;

		while (this->animation_time > this->frame_duration) {
			this->animation_time -= this->frame_duration;
		}
		while (this->animation_time < 0.0f) {
			this->animation_time += this->frame_duration;
		}

		// Figure out which frame we're on
		double frame_number = this->animation_time * frame_rate;
		int frame_index0 = static_cast<int>(floor(frame_number)) % frame_count;
		int frame_index1 = static_cast<int>(ceil(frame_number)) % frame_count;
		this->current_frame_index = frame_index0;

		float fInterpolate = static_cast<float>(fmod(this->animation_time, this->frame_duration));

		if (this->animation_file) {
			auto frame_skeleton = this->animation_file->InterpolateSkeletons(
				frame_index0, frame_index1, fInterpolate);
			this->bone_matrices.assign(frame_skeleton.bone_matrices.begin(), frame_skeleton.bone_matrices.end());
		}
	}

	void Animation::SetAnimationFile(std::shared_ptr<MD5Anim> file) {
		if (file) {
			this->animation_file = file;
			this->frame_count = this->animation_file->GetFrameCount();
			this->frame_rate = static_cast<float>(this->animation_file->GetFrameRate());
			this->frame_duration = 1.0f / this->frame_rate * this->frame_count;

			auto frame_skeleton = this->animation_file->InterpolateSkeletons(
				0, 1, 0.0f);
			this->bone_matrices.assign(frame_skeleton.bone_matrices.begin(), frame_skeleton.bone_matrices.end());
		}
	}
	void Animation::Out(proto::Component* target) {
		proto::Animation* comp = target->mutable_animation();
		comp->set_animation_name(this->animation_name);
	}

	extern std::map<std::string, std::function<void(std::string)>> file_factories;
	void Animation::In(const proto::Component& source) {
		const proto::Animation& comp = source.animation();
		if (comp.has_animation_name()) {
			this->animation_name = comp.animation_name();
			/*if (!AnimationMap::Has(this->animation_name)) {
				std::string ext = this->animation_name.substr(this->animation_name.find_last_of(".") + 1);
				if (file_factories.find(ext) != file_factories.end()) {
					file_factories[ext](this->animation_name);
				}
			}
			this->animation_file = AnimationMap::Get(this->animation_name);*/
		}
	}
}
