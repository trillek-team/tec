#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include <components.pb.h>

namespace tec {
class MD5Anim;

struct AnimationBone {
	glm::quat orientation;
	glm::vec4 offset;
	glm::vec4 rest;
};

class Animation final {
public:
	Animation() = default;
	Animation(std::shared_ptr<MD5Anim> animation);
	/**
	* \brief Updates the current animation based on a change in time.
	*
	* \param[in] float delta The change in time
	* \return void
	*/
	void UpdateAnimation(const double delta);

	/**
	* \brief Sets the animation file for this animation.
	*
	* This does a run for the first set of animation from frame 0 to 1 with time 0.
	* \param[in] std::shared_ptr<resource::MD5Anim> file The animation file.
	* \return void
	*/
	void SetAnimationFile(std::shared_ptr<MD5Anim> file);

	void Play();

	void Stop();

	void SetLoop(bool loop) { looping = loop; }

	void PlayOnce();

	void Pause();

	void Reset();

	void Out(proto::Component* target);
	void In(const proto::Component& source);

	bool HasBoneTransforms() const { return !bone_transforms.empty(); }

	const auto& GetBoneTransforms() { return this->bone_transforms; }

	friend class RenderSystem;

private:
	std::vector<AnimationBone> bone_transforms;

	std::string animation_name;
	std::shared_ptr<MD5Anim> animation_file;

	std::size_t current_frame_index = 0;
	std::size_t frame_count = 0;

	double animation_time{0.0};
	double animation_duration{0.0};
	double frame_duration{0.0};
	double frame_rate{0.0};
	double play_rate{1.0};
	bool looping{true};
};

} // namespace tec
