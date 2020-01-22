#pragma once

/*
该进前：
1. 对象用完之后需要手动回收，用起来不方便，存在忘记回收的风险。
2. 不支持参数不同的构造函数
*/

#include <string>
#include <functional>
#include <memory>
#include <map>
#include "NonCopyable.h"

const int MaxObjectNum = 10;

template <typename T>
class ObjectPool : NonCopyable
{
	template <typename... Args>
	using Constructor = std::function<std::shared_ptr<T>(Args...)>;
public:
template<typename... Args>
void Init(size_t num, Args&&... args) {
	if (num <= 0 || num > MaxObjectNum) {
		throw std::logic_error("object number is error");
	}
	auto constructName = typeid(Constructor<Args...>).name();
	for (size_t i = 0; i < num; i++) {
		m_object_map.emplace(constructName, shared_ptr<T>(new T(std::forward<Args>(args)...), [this, constructName](T* p)
		{
			m_object_map.emplace(std::move(constructName), std::shared_ptr<T>(p));
		}));
	}
}

template<typename... Args>
std::shared_ptr<T> Get() {
	std::string constructName = typeid(Constructor<Args...>).name();

	auto range = m_object_map.equal_range(constructName);

	for (auto it = range.first; it != range.second; ++it) {
		auto ptr = it->second;
		m_object_map.erase(it);
		return ptr;
	}
	return nullptr;
}
private:
	std::multimap<std::string, std::shared_ptr<T>> m_object_map;
};

