#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "filesystem.hpp"
#include "md5mesh.hpp"
#include "multiton.hpp"

namespace tec {
class MD5Mesh;
class MD5Anim;
struct AnimationBone;
typedef Multiton<std::string, std::shared_ptr<MD5Anim>> AnimationMap;

class MD5Anim final {
public:
	MD5Anim() = default;
	~MD5Anim() = default;

	/*****************************/
	/* MD5Anim helper structures */
	/*****************************/
	struct Joint {
		std::string name; // The name of the joint
		int parent{0}; // index
		int flags{0};
		int start_index{0};
		glm::vec3 base_position{0.f, 0.f, 0.f};
		glm::quat base_orientation{0.f, 0.f, 0.f, 1.f};
		glm::vec3 bind_position{0.f, 0.f, 0.f};
		glm::quat bind_orientation{0.f, 0.f, 0.f, 1.f};
	};

	struct BoundingBox {
		glm::vec3 min{0.f, 0.f, 0.f};
		glm::vec3 max{0.f, 0.f, 0.f};
	};

	struct SkeletonJoint {
		glm::vec3 position{0.f, 0.f, 0.f};
		glm::quat orientation{0.f, 0.f, 0.f, 1.f};
	};

	struct FrameSkeleton {
		std::vector<SkeletonJoint> skeleton_joints;
	};

	struct Frame {
		int index{0};
		std::vector<float> parameters;
		FrameSkeleton skeleton;
	};

	/**
	* \brief Returns a resource with the specified name.
	*
	* The only used initialization property is "filename".
	* \param[in] const Path& fname The filename of the MD5Anim resource
	* \return std::shared_ptr<MD5Anim> The create MD5Anim resource.
	*/
	static std::shared_ptr<MD5Anim> Create(const Path& fname);

	/**
	* \brief Loads the MD5Anim file from disk and parses it.
	*
	* \return bool If the animation was valid and loaded correctly.
	*/
	bool Parse();

	/**
	* \brief Sets the animation filename.
	*
	* This is just a shorthand function that can be called directly via script API.
	* \param[in] const std::string& fname The mesh filename.
	* \return bool True if initialization finished with no errors.
	*/
	void SetFileName(const Path& fname) { this->path = fname; }

	Path GetFileName() { return this->path; }

	const std::string GetName() const { return this->name; }

	void SetName(const std::string& _name) { this->name = _name; }

	/**
	* \brief Returns the number of animation frames.
	*
	* \return bool size_t The number of frames.
	*/
	std::size_t GetFrameCount() const { return this->frames.size(); }

	/**
	* \brief Returns the number of frame rate per second.
	*
	* \return int The frame rate per second.
	*/
	int GetFrameRate() const { return this->frame_rate; }

	/**
	* \brief Builds or rebuilds the FrameSkeleton for a given frame index.
	*
	* \param[in] size_t frame_index Index of the frame to build/rebuild.
	* \return void
	*/
	void BuildFrameSkeleton(std::size_t frame_index);

	/**
	* \brief Checks of a given mesh is valid for this animation.
	*
	* \param[in] std::shared_ptr<Mesh> mesh_file The mesh to check against.
	* \return bool True if the mesh is compatible.
	*/
	bool CheckMesh(std::shared_ptr<MD5Mesh> mesh_file);

	/**
	* \brief Gets the interpolated pose skeleton between 2 frames at a given delta.
	*
	* \param[out] pose_out array of AnimationBone to store the interpolated pose in.
	* \param[in] frame_index_start The starting frame index.
	* \param[in] frame_index_end The ending frame index.
	* \param[in] delta The change in time since the last call.
	*/
	void InterpolatePose(
			std::vector<AnimationBone>& pose_out,
			std::size_t frame_index_start,
			std::size_t frame_index_end,
			float delta);

private:
	Path path; // Relative filename
	std::string name;
	std::vector<BoundingBox> bounds; // Bound box sizes for each join.
	std::vector<Frame> frames;
	std::vector<Joint> joints;
	int frame_rate{0}; // Frame rate.
};
} // namespace tec
