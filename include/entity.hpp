#pragma once

#include <memory>
#include <tuple>
#include "multiton.hpp"
#include "component-update-system.hpp"

namespace tec {
	class Entity {
	public:
		Entity(eid id) : id(id) { }

		template <typename T, typename... U>
		void Add(U&&... args) {
			if (!Multiton<eid, std::shared_ptr<T>>::Has(this->id)) {
				auto comp = std::make_shared<T>(std::forward<U>(args)...);

				Update(comp);
			}
		}
		template <typename T>
		void Add(std::shared_ptr<T> comp) {
			Update(comp);
		}

		template <typename T>
		void Remove() {
			ComponentUpdateSystem<T>::SubmitRemoval(this->id);
		}

		template <typename T>
		bool Has() {
			return Multiton<eid, std::shared_ptr<T>>::Has(this->id);
		}

		template <typename T>
		std::weak_ptr<T> Get() {
			return Multiton<eid, std::shared_ptr<T>>::Get(this->id);
		}

		template <typename... T>
		std::tuple<std::weak_ptr<T>...> GetList() {
			return std::make_tuple(Multiton<eid, std::shared_ptr<T>>::Get(this->id)...);
		}

		template <typename T>
		void Update(std::shared_ptr<T> val) {
			ComponentUpdateSystem<T>::SubmitUpdate(this->id, val);
		}

		template <typename T>
		void Update(T val) {
			auto comp = std::make_shared<T>(val);
			Update(comp);
		}

		eid GetID() {
			return this->id;
		}
	private:
		eid id;
	};
}
