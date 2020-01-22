#pragma once

/*
命令模式
1. 命令模式是行为模式的一种。Command模式通过被称为Command的类封装了对目标对象的调用行为以及调用参数
*/

#include <functional>
#include <type_traits>
#include <iostream>

template <typename R = void>
struct CommCommand {
private:
	std::function<R()> m_f;
public:
	//接送可调用函数对象包装器
	template <class F,class... Args,class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
	void Wrap(F &&f, Args &&... args) {
		m_f = [&] {
			return f(args...); 
		};
	}
	//接受常量成员函数的函数包装器
	template<class R,class C,class... DArgs,class P,class... Args>
	void Wrap ( R(C::*f)(DArgs...) const, P &&p, Args && ...args) {
		m_f = [&, f] {
			return (*p.*f)( args...);
		};
	}
	//接受非常量成员函数的函数包装器
	template<class R,class C,class... DArgs,class P,class... Args>
	void Wrap(R(C::*f)(DArgs...), P && p, Args && ...args) {
		m_f = [&, f] {
			return (*p.*f)(args...); 
		};
	}

	R Excecute() {
		return m_f();
	}
};

struct STA {
	int m_a;
	int operator()() {
		return m_a;
	}

	int operator()(int n) {
		return m_a + n;
	}

	int triple0() {
		return m_a * 3;
	}

	int triple(int a) {
		return m_a * 3 + a;
	}

	int triple1() const {
		return m_a * 3;
	}

	const int triple2(int a) const {
		return m_a * 3 + a;
	}

	void triple3() {
		std::cout << "" << std::endl;
	}
};

