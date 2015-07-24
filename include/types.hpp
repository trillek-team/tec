#pragma once

#include <cstdint>

namespace tec {
	typedef std::int64_t frame_id_t;

	typedef std::int64_t GUID;

	typedef std::int64_t eid;

	template<class TYPE> const char* GetTypeName(void) { return "UNKNOWN"; }
	template<class TYPE> unsigned int GetTypeID(void) { return ~0; }
	template<class TYPE> const char* GetTypeEXT(void) { return "UNKNOWN"; }

#define MAKE_IDTYPE(a,b) \
	template<> inline const char* GetTypeName<a>() { return #a; } \
	template<> inline unsigned int GetTypeID<a>() { return b; }
#define MAKE_EXTTYPE(a,b) \
	template<> inline const char* GetTypeName<a>() { return #a; } \
	template<> inline const char* GetTypeEXT<a>() { return b; }
#define MAKE_IDTYPE_NAMESPACE(ns,a,b) \
	template<> inline const char* GetTypeName<ns::a>() { return #a; } \
	template<> inline unsigned int GetTypeID<ns::a>() { return b; }

	struct Renderable;
	MAKE_IDTYPE(Renderable, 0);
	struct Position;
	MAKE_IDTYPE(Position, 1);
	struct Orientation;
	MAKE_IDTYPE(Orientation, 2);
	struct View;
	MAKE_IDTYPE(View, 3);
	class Animation;
	MAKE_IDTYPE(Animation, 4);
	struct Scale;
	MAKE_IDTYPE(Scale, 5);
	struct CollisionBody;
	MAKE_IDTYPE(CollisionBody, 6);
	struct Velocity;
	MAKE_IDTYPE(Velocity, 7);


	class MD5Mesh;
	MAKE_EXTTYPE(MD5Mesh, "md5mesh");
	class OBJ;
	MAKE_EXTTYPE(OBJ, "obj");
}
