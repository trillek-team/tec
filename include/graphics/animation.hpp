#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace tec {
	class MD5Anim;

	class Animation final {
	public:
		Animation() : animation_time(0.0f) { }
		Animation(std::shared_ptr<MD5Anim> animation) : animation_time(0.0f) {
			SetAnimationFile(animation);
		}

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

		friend class RenderSystem;
	private:
		std::vector<glm::mat4x4> animation_matrices;

		std::shared_ptr<MD5Anim> animation_file;

		size_t current_frame_index;
		size_t frame_count;

		double animation_time;
		double frame_duration;
		double frame_rate;
	};

}
