#pragma once

/**
 * @file types.hpp
 * Here we define some typedefs of some usual types used across the whole project, plus some
 * templated helper functions for reflection
 */

#include <cstdint>
#include <unordered_map>

#include <components.pb.h>

// forward declare sol::state for RegisterLuaType
// this avoids including sol/sol.hpp everywhere (heavy!)
namespace sol {
class state;
}

namespace tec {
typedef std::uint64_t state_id_t;
#define PRI_STATE_ID_T PRId64

typedef std::int64_t GUID; /// Global Unique ID

typedef std::uint64_t eid; /// Entity ID
#define PRI_EID PRId64

typedef std::uint32_t tid; /// Type ID

typedef std::string uid; /// User ID

template <class T> bool IsBetween(T value, T lower, T upper) { return (value > lower) && (value < upper); }

/// Returns the name of an component on compile time
template <class TYPE> constexpr const char* GetTypeName(void) { return "UNKNOWN"; }
/// Returns the TypeID of a component on compile time
template <class TYPE> constexpr tid GetTypeID(void) { return proto::Component::COMPONENT_NOT_SET; }
/// Returns the name of a resource type on Compile time
template <class TYPE> constexpr const char* GetTypeEXT(void) { return "UNKNOWN"; }

/*
	 * Use this macro to associate a component to an type id and generate a string with the name
	 * Only works if the struct and component name on .proto file are the same (see the list on message Component )
	 */
#define MAKE_IDTYPE(a) \
	template <> inline constexpr const char* GetTypeName<a>() { return #a; } \
	template <> inline constexpr tid GetTypeID<a>() { return proto::Component::k##a; }

#define MAKE_IDTYPE_NAMESPACE(ns, a, b) \
	template <> inline constexpr const char* GetTypeName<ns::a>() { return #a; } \
	template <> inline constexpr tid GetTypeID<ns::a>() { return proto::Component::k##a; }

#define MAKE_EXTTYPE(a, b) \
	template <> inline constexpr const char* GetTypeName<a>() { return #a; } \
	template <> inline constexpr const char* GetTypeEXT<a>() { return b; }

// Registering components

struct Renderable;
MAKE_IDTYPE(Renderable); //kRenderable of proto::Component enum

struct Position;
MAKE_IDTYPE(Position);

struct Orientation;
MAKE_IDTYPE(Orientation);

struct View;
MAKE_IDTYPE(View);

class Animation;
MAKE_IDTYPE(Animation);

struct Scale;
MAKE_IDTYPE(Scale);

struct CollisionBody;
MAKE_IDTYPE(CollisionBody);

struct Velocity;
MAKE_IDTYPE(Velocity);

struct AudioSource;
MAKE_IDTYPE(AudioSource);

struct PointLight;
MAKE_IDTYPE(PointLight);

struct DirectionalLight;
MAKE_IDTYPE(DirectionalLight);

struct Computer;
MAKE_IDTYPE(Computer);

// SpotLight ??

class VoxelVolume;
MAKE_IDTYPE(VoxelVolume);

struct LuaScript;
MAKE_IDTYPE(LuaScript);

// Register Resource file types

class MD5Mesh;
MAKE_EXTTYPE(MD5Mesh, "md5mesh");

class MD5Anim;
MAKE_EXTTYPE(MD5Anim, "md5anim");

class OBJ;
MAKE_EXTTYPE(OBJ, "obj");

class VorbisStream;
MAKE_EXTTYPE(VorbisStream, "ogg");

class ScriptFile;
MAKE_EXTTYPE(ScriptFile, "lua");

/// Maps on runtime the Type ID with the name
extern const std::unordered_map<tid, const char*> TypeName;

#undef MAKE_EXTTYPE
#undef MAKE_IDTYPE_NAMESPACE
#undef MAKE_IDTYPE

} // end namespace tec
