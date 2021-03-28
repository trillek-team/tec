#pragma once
namespace tec {
namespace util {

template <typename T> class DataSource {
public:
	virtual void Add(T value) = 0;
	virtual void RemoveWhere(std::function<bool(const T&)> predicate) = 0;
	virtual T* FindWhere(std::function<bool(const T&)> predicate) = 0;
};

} // namespace util
} // namespace tec
