#pragma once

/*
����ģʽ
1. ����ģʽ����Ϊģʽ��һ�֡�Commandģʽͨ������ΪCommand�����װ�˶�Ŀ�����ĵ�����Ϊ�Լ����ò���
*/

#include <functional>
#include <type_traits>
#include <iostream>

template <typename R = void>
struct CommCommand {
private:
	std::function<R()> m_f;
public:
	//���Ϳɵ��ú��������װ��
	template <class F,class... Args,class = typename std::enable_if<!std::is_member_function_pointer<F>::value>::type>
	void Wrap(F &&f, Args &&... args) {
		m_f = [&] {
			return f(args...); 
		};
	}
	//���ܳ�����Ա�����ĺ�����װ��
	template<class R,class C,class... DArgs,class P,class... Args>
	void Wrap ( R(C::*f)(DArgs...) const, P &&p, Args && ...args) {
		m_f = [&, f] {
			return (*p.*f)( args...);
		};
	}
	//���ܷǳ�����Ա�����ĺ�����װ��
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

