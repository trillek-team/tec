#pragma once

#include <memory>
#include <map>
#include <string>
#include <functional>
#include "types.hpp"

namespace tec {
	typedef std::pair<std::function<std::vector<std::string>&(void)>, std::string> dropdown_t;
	struct Property {
		enum Type {
			INTEGER,
			FLOAT,
			BOOLEAN,
			STRING,
			COLOR,
			VEC3,
			DROPDOWN,
		};
		Type type;
		Property(Type t = INTEGER) : type(t), value_holder(nullptr) { }
		Property(const Property &other) {
			this->type = other.type;
			this->update_func = other.update_func;
			if (other.value_holder != nullptr) {
				this->value_holder = other.value_holder->Clone();
			}
			else {
				this->value_holder = nullptr;
			}
		}
		Property& operator= (const Property &other) {
			if (this != &other) {
				this->type = other.type;
				this->update_func = other.update_func;
				if (other.value_holder != nullptr) {
					this->value_holder = other.value_holder->Clone();
				}
				else {
					this->value_holder = nullptr;
				}
			}
			return *this;
		}
		Property(Property&& other) : type(other.type), value_holder(other.value_holder), update_func(other.update_func) {
			other.value_holder = nullptr;
		}
		~Property() { if (this->value_holder) { delete this->value_holder; } }

		template <typename T>
		T Get() { return static_cast<ValueHider<T>*>(this->value_holder)->Get(); }
		template <typename T>
		void Set(T v) {
			if (this->value_holder) {
				delete this->value_holder;
			}
			this->value_holder = new ValueHider<T>(v);
		}
		std::function<void(Property&)> update_func;
	private:
		class ValueHolderBase {
		public:
			virtual ~ValueHolderBase() { }
			virtual ValueHolderBase* Clone() const = 0;
		};
		template <typename T>
		class ValueHider : public ValueHolderBase {
		public:
			ValueHider(T v) : value(v) { }
			virtual ValueHider* Clone() const { return new ValueHider(this->value); }
			T Get() { return this->value; }
		private:
			T value;
		};
		ValueHolderBase* value_holder;
	};
	struct ReflectionComponent {
		std::map<std::string, Property> properties;
	};
	struct ReflectionEntity {
		std::map<std::string, ReflectionComponent> components;
	};
	struct ReflectionEntityList {
		std::map<eid, ReflectionEntity> entities;
	};
}
