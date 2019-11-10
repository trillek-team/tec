// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <map>
#include <vector>
#include "types.hpp"

namespace tec {
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
	template <typename ID_T, typename T>
	class Multiton {
	public:
		static typename std::map<ID_T, T>::iterator Begin() {
			return instances.begin();
		}

		static typename std::map<ID_T, T>::iterator End() {
			return instances.end();
		}

		static typename std::size_t Size() {
			return instances.size();
		}

		static typename std::vector<ID_T>& Keys() {
			static std::vector<ID_T> keys;
			if (keys.size() != instances.size()) {
				keys.clear();
				for (auto pair : instances) {
					keys.push_back(pair.first);
				}
			}
			return keys;
		}

		/**
		* \brief Get the instance for the given ID.
		*
		* This doesn't create an instance if the ID doesn't exist.
		* Instead it just returns the default.
		* \param[in] const ID The ID of the instance to get.
		* \return T The ID's instance or the default one.
		*/
		static T Get(const ID_T id) {
			if (instances.find(id) != instances.end()) {
				return instances.at(id);
			}
			return default_value;
		}


		static bool Has(const ID_T id) {
			return instances.find(id) != instances.end();
		}

		/**
		* \brief Set (or add/create) an instance for the given ID.
		*
		* \param[in] const ID id The ID of the instance to set.
		* \param[in] T instance The ID's instance.
		* \return void
		*/
		static void Set(const ID_T id, T instance) {
			instances[id] = instance;
		}

		/**
		* \brief Remove the instance for the given ID.
		*
		* \param[in] const ID id The ID of the instance to remove.
		* \return void
		*/
		static void Remove(const ID_T id) {
			instances.erase(id);
		}
	protected:
		static T default_value; // Default instance.

		// TODO: Replace this with a weak_ptr to allow pruning?
		static std::map<ID_T, T> instances; // Mapping of ID to instance.
	};


	template <typename ID_T, typename T>
	std::map<ID_T, T> Multiton<ID_T, T>::instances;

	template <typename ID_T, typename T>
	T Multiton<ID_T, T>::default_value;
}
