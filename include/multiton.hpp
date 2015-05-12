#pragma once

#include <map>
#include "types.hpp"

namespace vv {
	/* Based class to be used for instance or state storage.
	*
	* This is useful in place of static instances or globals. Derived classes
	* can operate on the static instance member using instances of the derived
	* class. This could allow, for example, and animation controller to derive
	* from this and have each instance use a separate animation sequence, but
	* all animation instance data would be stored in one map.
	*
	* Additionally by encapsulating the maps in this pattern we provide better
	* data hiding and can better control access methods to the instance data.
	*
	* Modeled after http://en.wikipedia.org/wiki/Multiton_pattern.
	*/
	template <typename ID, typename T>
	class Multiton {
	public:
		/**
		* \brief Get the default instance.
		*
		* \return T The default instance.
		*/
		static T Default() {
			return default_value;
		}

		/**
		* \brief Set the default instance.
		*
		* \return void
		*/
		static void Default(const T value) {
			default_value = value;
		}

		static typename std::map<ID, T>::const_iterator Begin() {
			return instances.begin();
		}

		static typename std::map<ID, T>::const_iterator End() {
			return instances.end();
		}

		/**
		* \brief Get the instance for the given ID.
		*
		* This doesn't create an instance if the ID doesn't exist.
		* Instead it just returns the default.
		* \param[in] const ID The ID of the instance to get.
		* \return T The ID's instance or the default one.
		*/
		static T Get(const ID id) {
			if (instances.find(id) != instances.end()) {
				return instances.at(id);
			}
			return default_value;
		}


		static bool Has() {
			return instances.find(id) != instances.end();
		}

		/**
		* \brief Set (or add/create) an instance for the given ID.
		*
		* \param[in] const ID id The ID of the instance to set.
		* \param[in] T instance The ID's instance.
		* \return void
		*/
		static void Set(const ID id, const T instance) {
			instances[id] = instance;
		}

		/**
		* \brief Remove the instance for the given ID.
		*
		* \param[in] const ID id The ID of the instance to remove.
		* \return void
		*/
		static void Remove(const ID id) {
			instances.erase(id);
		}
	protected:
		static T default_value; // Default instance.

		// TODO: Replace this with a weak_ptr to allow pruning?
		static std::map<ID, T> instances; // Mapping of ID to instance.
	};


	template <typename ID, typename T>
	std::map<ID, T> Multiton<ID, T>::instances;

	template <typename ID, typename T>
	T Multiton<ID, T>::default_value;
}
