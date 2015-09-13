#pragma once

#include <cstdint>
#include "../proto/components.pb.h"

namespace tec {
	typedef std::int64_t frame_id_t;

	typedef std::int64_t GUID;

	typedef std::int64_t eid;

	template<class TYPE> const char* GetTypeName(void) { return "UNKNOWN"; }
	template<class TYPE> proto::Component::ComponentCase GetComponentCase(void) { return proto::Component::ComponentCase::COMPONENT_NOT_SET; }
	template<class TYPE> unsigned int GetTypeID(void) { return ~0; }
	template<class TYPE> const char* GetTypeEXT(void) { return "UNKNOWN"; }

#define MAKE_IDTYPE(a,b) \
	template<> inline const char* GetTypeName<a>() { return #a; } \
	template<> inline unsigned int GetTypeID<a>() { return b; }
#define MAKE_COMPONENTCASETYPE(a,b) \
	template<> inline proto::Component::ComponentCase GetComponentCase<a>() { return b; }
#define MAKE_EXTTYPE(a,b) \
	template<> inline const char* GetTypeName<a>() { return #a; } \
	template<> inline const char* GetTypeEXT<a>() { return b; }
#define MAKE_IDTYPE_NAMESPACE(ns,a,b) \
	template<> inline const char* GetTypeName<ns::a>() { return #a; } \
	template<> inline unsigned int GetTypeID<ns::a>() { return b; }

	struct Renderable;
	MAKE_IDTYPE(Renderable, 0);
	MAKE_COMPONENTCASETYPE(Renderable, proto::Component::kRenderable);
	struct Position;
	MAKE_IDTYPE(Position, 1);
	MAKE_COMPONENTCASETYPE(Position, proto::Component::kPosition);
	struct Orientation;
	MAKE_IDTYPE(Orientation, 2);
	MAKE_COMPONENTCASETYPE(Orientation, proto::Component::kOrientation);
	struct View;
	MAKE_IDTYPE(View, 3);
	MAKE_COMPONENTCASETYPE(View, proto::Component::kView);
	class Animation;
	MAKE_IDTYPE(Animation, 4);
	MAKE_COMPONENTCASETYPE(Animation, proto::Component::kAnimation);
	struct Scale;
	MAKE_IDTYPE(Scale, 5);
	MAKE_COMPONENTCASETYPE(Scale, proto::Component::kScale);
	struct CollisionBody;
	MAKE_IDTYPE(CollisionBody, 6);
	MAKE_COMPONENTCASETYPE(CollisionBody, proto::Component::kCollisionBody);
	struct Velocity;
	MAKE_IDTYPE(Velocity, 7);
	MAKE_COMPONENTCASETYPE(Velocity, proto::Component::kVelocity);
	struct AudioSource;
	MAKE_IDTYPE(AudioSource, 8);
	MAKE_COMPONENTCASETYPE(AudioSource, proto::Component::kAudioSource);
	struct PointLight;
	MAKE_IDTYPE(PointLight, 9);
	MAKE_COMPONENTCASETYPE(PointLight, proto::Component::kLight);
	struct DirectionalLight;
	MAKE_IDTYPE(DirectionalLight, 10);
	MAKE_COMPONENTCASETYPE(DirectionalLight, proto::Component::kDirlight);
	class VoxelVolume;
	MAKE_IDTYPE(VoxelVolume, 11);

	class MD5Mesh;
	MAKE_EXTTYPE(MD5Mesh, "md5mesh");
	class OBJ;
	MAKE_EXTTYPE(OBJ, "obj");
	class VorbisStream;
	MAKE_EXTTYPE(VorbisStream, "ogg");
}
