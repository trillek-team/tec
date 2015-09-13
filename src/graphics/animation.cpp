#include "graphics/animation.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "resources/md5anim.hpp"

namespace tec {
	Animation::Animation(std::shared_ptr<MD5Anim> animation) : animation_time(0.0f), frame_count(0) {
		SetAnimationFile(animation);
		this->animation_name = animation_file->GetName();
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
		int frame_index0 = (int)floor(frame_number);
		int frame_index1 = (int)ceil(frame_number);
		frame_index0 = frame_index0 % frame_count;
		frame_index1 = frame_index1 % frame_count;
		this->current_frame_index = frame_index0;

		float fInterpolate = static_cast<float>(fmod(this->animation_time, this->frame_duration));

		if (this->animation_file) {
			auto frame_skeleton = this->animation_file->InterpolateSkeletons(
				frame_index0, frame_index1, fInterpolate);
			this->animation_matrices.assign(frame_skeleton.bone_matricies.begin(), frame_skeleton.bone_matricies.end());
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
			this->animation_matrices.assign(frame_skeleton.bone_matricies.begin(), frame_skeleton.bone_matricies.end());
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

	ReflectionComponent Animation::Reflection(Animation* val) {
		ReflectionComponent refcomp;
		Property sprop(Property::STRING);
		(refcomp.properties["Animation Name"] = sprop).Set<std::string>(val->animation_name);
		refcomp.properties["Animation Name"].update_func = [val] (Property& prop) { val->animation_name = prop.Get<std::string>(); };
		Property iprop(Property::INTEGER);
		(refcomp.properties["Current Frame"] = sprop).Set<int>(val->current_frame_index);
		refcomp.properties["Current Frame"].update_func = [val] (Property& prop) { val->current_frame_index = prop.Get<int>(); };
		return std::move(refcomp);
	}
}
