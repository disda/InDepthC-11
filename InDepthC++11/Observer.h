#pragma once

/*
C++11改进观察者模式：
1. 通知接口参数化
2. std::function 来替代继承

只需將函數注册进来，消除了继承导致的强耦合性，通知接口使用了可变参数模版。
*/

#include <list>
class Subject;

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void Update(Subject* theChangeSubject) = 0;
};

class Subject {
public:
	virtual ~Subject();
	virtual void Attach(Observer*);
	virtual void Detach(Observer*);
	virtual void Notify();
private:
	std::list<Observer*> m_observers;
};

#include "NonCopyable.h"
#include <iostream>
#include <string>
#include <functional>
#include <map>

using namespace std;

template <typename Func>
class Events : NonCopyable {
public:
	Events() = default;
	~Events() = default;
	int Connect(Func&& f) {
		return _assgin(f);
	}

	int Connect(const Func& f) {
		return _assgin(f);
	}

	void Disconnect(int k) {
		m_connections.erase(k);
	}

	//通知所有的观察者
	template<typename... Args>
	void Notify(Args&&... args) {
		for (auto it : m_connections) {
			it.second(std::forward<Args>(args)...);
		}
	}

private:
	template <typename T>
	int _assgin(T&& f) {
		int k = m_observerId++;
		m_connections.emplace(k, std::forward<T>(f));
		return k;
	}

	int m_observerId = 0;
	std::map<int, Func> m_connections;
};