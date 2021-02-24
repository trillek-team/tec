#pragma once

#include <tuple>

#include "multiton.hpp"

namespace tec {
class Entity {
public:
	Entity(eid id) : id(id) {}

	// Add a component with constructor arguments.
	template <typename T, typename... U> T* Add(U&&... args) {
		if (!Multiton<eid, T*>::Has(this->id)) {
			T* comp = new T(std::forward<U>(args)...);

			Update(comp);
			return comp;
		}
		return Multiton<eid, T*>::Get(this->id);
	}

	// Add multiple components at one.
	// Returns a tuple of each added component in the order they were specified in the template.
	template <typename... T> std::tuple<T*...> Add() {
		int _[] = {0, (Update(new T()), 0)...};
		(void)_;
		return std::make_tuple(Multiton<eid, T*>::Get(this->id)...);
	}

	// Add multiple components at one with constructor arguments (1 per component).
	// Returns a tuple of each added component in the order they were specified in the template.
	template <typename... T> std::tuple<T*...> Add(T... args) {
		int _[] = {0, (Update(new T(args)), 0)...};
		(void)_;
		return std::make_tuple(Multiton<eid, T*>::Get(this->id)...);
	}

	// Gets all the specified components and serializes them to a proto::Entity.
	template <typename... T> void Out(proto::Entity& entity) {
		int _[] = {0, (Multiton<eid, T*>::Get(this->id)->Out(entity.add_components()), 0)...};
		(void)_;
	}

	// Add a pre-made component to this entity.
	template <typename T> void Add(T* comp) { Update(comp); }

	// Remove a specific component from this entity.
	template <typename T> void Remove() { Multiton<eid, T*>::Remove(this->id); }

	// Checks if this entity has a specific component.
	template <typename T> bool Has() { return Multiton<eid, T*>::Has(this->id); }

	// Get a specific component.
	template <typename T> const T* Get() { return Multiton<eid, T*>::Get(this->id); }

	// Get multiple components.
	template <typename... T> std::tuple<const T*...> GetList() {
		return std::make_tuple(Multiton<eid, T*>::Get(this->id)...);
	}

	// Sets a component to the provided component.
	template <typename T> void Update(T* val) {
		delete Multiton<eid, T*>::Get(this->id);
		Multiton<eid, T*>::Set(this->id, val);
	}

	// Get the entity id.
	eid GetID() { return this->id; }

private:
	eid id;
};
} // namespace tec
