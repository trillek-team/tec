#include "animation.hpp"

#include <functional>

#include "file-factories.hpp"
#include "resources/md5anim.hpp"
#include "resources/md5mesh.hpp"

namespace tec {
Animation::Animation(std::shared_ptr<MD5Anim> animation) {
	SetAnimationFile(animation);
	if (animation) {
		this->animation_name = animation_file->GetName();
	}
}
void Animation::UpdateAnimation(const double delta) {
	if (this->frame_count < 1) {
		return;
	}
	if (play_rate == 0.0) {
		return;
	}
	this->animation_time += delta * play_rate;

	if (this->animation_time > this->animation_duration) {
		if (looping) {
			this->animation_time -= this->animation_duration;
		}
		else {
			this->animation_time = this->animation_duration;
			play_rate = 0.0;
		}
	}
	if (this->animation_time < 0.0f) {
		if (looping) {
			this->animation_time += this->animation_duration;
		}
		else {
			this->animation_time = 0.0;
			play_rate = 0.0;
		}
	}

	// Figure out which frame we're on
	const double frame_number = this->animation_time * this->frame_rate;
	const std::size_t frame_index0 = static_cast<std::size_t>(floor(frame_number)) % frame_count;
	const std::size_t frame_index1 = static_cast<std::size_t>(ceil(frame_number)) % frame_count;
	this->current_frame_index = frame_index0;

	const float fInterpolate = static_cast<float>(glm::fract(frame_number));

	if (this->animation_file) {
		this->animation_file->InterpolatePose(this->bone_transforms, frame_index0, frame_index1, fInterpolate);
	}
	else {
		this->bone_transforms.clear();
	}
}

void Animation::SetAnimationFile(std::shared_ptr<MD5Anim> file) {
	if (file) {
		this->animation_file = file;
		this->frame_count = this->animation_file->GetFrameCount();
		this->frame_rate = static_cast<double>(this->animation_file->GetFrameRate());
		this->frame_duration = 1.0 / this->frame_rate;
		this->animation_duration = this->frame_duration * static_cast<double>(this->frame_count);

		this->animation_file->InterpolatePose(this->bone_transforms, 0, 1, 0.0f);
	}
}
void Animation::Out(proto::Component* target) {
	proto::Animation* comp = target->mutable_animation();
	comp->set_animation_name(this->animation_name);
}

void Animation::In(const proto::Component& source) {
	const proto::Animation& comp = source.animation();
	if (!comp.has_animation_name()) {
		return;
	}
	if (comp.play()) {
		this->play_rate = 1.0;
	}
	this->looping = comp.loop();
	this->animation_name = comp.animation_name();
	const auto mesh_name = comp.mesh_name();
	const auto animation{GetResource<MD5Anim>(this->animation_name)};
	if (animation && animation->CheckMesh(std::static_pointer_cast<MD5Mesh>(GetResource<MeshFile>(mesh_name)))) {
		this->SetAnimationFile(animation);
	}
}
} // namespace tec
