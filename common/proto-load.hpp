#pragma once

#include <game_state.pb.h>

#include "event-system.hpp"
#include "events.hpp"
#include "filesystem.hpp"

namespace tec {

// Load the contents of a file as a string
std::string LoadAsString(const Path& fname);

// Save a string directly to a file
bool SaveFromString(const Path& fname, std::string contents);

// Loads a given json file into a reference
bool LoadProtoPack(const Path& fname, proto::Entity& entity);

// Loads a given json file and emits it into the system
void ProtoLoadEntity(const Path& fname);

} // namespace tec
