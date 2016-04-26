#pragma once

#include <memory>
#include <tuple>
#include "multiton.hpp"

namespace tec {
	class Entity {
	public:
		Entity(eid id) : id(id) { }

		// Add a component with constructor arguments.
		template <typename T, typename... U>
		std::shared_ptr<T> Add(U&&... args) {
			if (!Multiton<eid, std::shared_ptr<T>>::Has(this->id)) {
				auto comp = std::make_shared<T>(std::forward<U>(args)...);

				Update(comp);
				return comp;
			}
			return Multiton<eid, std::shared_ptr<T>>::Get(this->id);
		}

		// Add multiple components at one.
		// Returns a tuple of each added component in the order they were specified in the template.
		template <typename... T>
		std::tuple<std::shared_ptr<T>...> Add() {
			int _[] = {0, (Update(std::make_shared<T>()), 0)...};
			(void)_;
			return std::make_tuple(Multiton<eid, std::shared_ptr<T>>::Get(this->id)...);
		}

		// Add multiple components at one with constructor arguments (1 per component).
		// Returns a tuple of each added component in the order they were specified in the template.
		template <typename... T>
		std::tuple<std::shared_ptr<T>...> Add(T... args) {
			int _[] = {0, (Update(std::make_shared<T>(args)), 0)...};
			(void)_;
			return std::make_tuple(Multiton<eid, std::shared_ptr<T>>::Get(this->id)...);
		}

		// Gets all the specified components and serializes them to a proto::Entity.
		template <typename... T>
		void Out(proto::Entity& entity) {
			int _[] = {0, (Multiton<eid, std::shared_ptr<T>>::Get(this->id)->Out(entity.add_components()), 0)...};
			(void)_;
		}

		// Add a pre-made component to this entity.
		template <typename T>
		void Add(std::shared_ptr<T> comp) {
			Update(comp);
		}
		
		// Remove a specific component from this entity.
		template <typename T>
		void Remove() {
			Multiton<eid, std::shared_ptr<T>>::Remove(this->id);
		}

		// Checks if this entity has a specific component.
		template <typename T>
		bool Has() {
			return Multiton<eid, std::shared_ptr<T>>::Has(this->id);
		}

		// Get a specific component.
		template <typename T>
		std::weak_ptr<T> Get() {
			return Multiton<eid, std::shared_ptr<T>>::Get(this->id);
		}

		// Get multiple components.
		template <typename... T>
		std::tuple<std::weak_ptr<T>...> GetList() {
			return std::make_tuple(Multiton<eid, std::shared_ptr<T>>::Get(this->id)...);
		}

		// Sets a component to the provided component.
		template <typename T>
		void Update(std::shared_ptr<T> val) {
			Multiton<eid, std::shared_ptr<T>>::Set(this->id, val);
		}

		// Get the entity id.
		eid GetID() {
			return this->id;
		}
	private:
		eid id;
	};
}
