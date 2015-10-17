/**
 * @file types.cpp
 *
 * Here defines the values of string typename of components, on runtime (ie when we can't use template T parameters )
 */

#include "types.hpp"

namespace tec {
	
// Tool to build the runtime name storage of components
#define MAKE_MAPPAIR(a) \
{ GetTypeID<a>(), GetTypeName<a>() }

	/// Maps on runtime the Type ID with the name
	const std::unordered_map<tid, const char*> TypeName = {
		MAKE_MAPPAIR(void) ,
		MAKE_MAPPAIR(Renderable) ,
		MAKE_MAPPAIR(Position) ,
		MAKE_MAPPAIR(Orientation) ,
		MAKE_MAPPAIR(View) ,
		MAKE_MAPPAIR(Animation) ,
		MAKE_MAPPAIR(Scale) ,
		MAKE_MAPPAIR(CollisionBody) ,
		MAKE_MAPPAIR(AudioSource) ,
		MAKE_MAPPAIR(PointLight) ,
		MAKE_MAPPAIR(DirectionalLight) ,
		// SpotLight
		MAKE_MAPPAIR(VoxelVolume)
		MAKE_MAPPAIR(LuaScript)
	};

#undef MAKE_MAPPAIR
}

