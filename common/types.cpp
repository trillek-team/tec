// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

/**
 * @file types.cpp
 *
 * Here defines the values of string typename of components, on runtime (ie when we can't use
 * template T parameters )
 */

#define __TYPES_CPP_
#include "types.hpp"

namespace tec {

// Tool to build the runtime name storage of components
#define MAKE_MAPPAIR(a) \
	{ GetTypeID<a>(), GetTypeName<a>() }

	/// Maps on runtime the Type ID with the name
	const std::unordered_map<tid, const char*> TypeName = { MAKE_MAPPAIR(void),
		MAKE_MAPPAIR(Renderable), MAKE_MAPPAIR(Position), MAKE_MAPPAIR(Orientation),
		MAKE_MAPPAIR(View), MAKE_MAPPAIR(Animation), MAKE_MAPPAIR(Scale),
		MAKE_MAPPAIR(CollisionBody), MAKE_MAPPAIR(Velocity), MAKE_MAPPAIR(AudioSource),
		MAKE_MAPPAIR(PointLight), MAKE_MAPPAIR(DirectionalLight),
		// SpotLight
		MAKE_MAPPAIR(VoxelVolume), MAKE_MAPPAIR(Computer), MAKE_MAPPAIR(LuaScript) };

#undef MAKE_MAPPAIR
} // namespace tec
