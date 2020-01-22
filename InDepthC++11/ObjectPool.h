#pragma once

/*
�ý�ǰ��
1. ��������֮����Ҫ�ֶ����գ������������㣬�������ǻ��յķ��ա�
2. ��֧�ֲ�����ͬ�Ĺ��캯��
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

