#include <iostream>
#include <vector>

#include "Singleton.h"
#include "Observer.h"

struct A {
	A() {
		std::cout << "A()" << std::endl;
	}
};

struct B {
	B(int x) {
		std::cout << "B()" << x << std::endl;
	}
};

struct C {
	C(int x, double y) {
		std::cout << "C()" << x << " " << y << std::endl;
	}
};

/*
1. 递归模板函数
2. 逗号表达式和初始化列表方式展开参数包
*/
#if 1 

template <class T>
void printarg(T t) {
	std::cout << t << std::endl;
}

template <class... Args> 
void expand(Args... args) {
	int arr[] = { (printarg(args),0)... };
}

template <class... T>
void f(T... args) {
	std::cout << sizeof...(args) << std::endl;
	//int arr[] = { (printarg(args),0)... };
	std::initializer_list<int>{ (printarg(args), 0)... };
}
#endif

struct stA {
	int a, b;
	void print(int a, int b) {
		std::cout << a << "," << b << std::endl;
	}
};

void print(int a, int b) {
	std::cout << a << ", " << b << std::endl;
}


int main() {
	Singleton<A>::Instance();
	Singleton<B>::Instance(1);
	Singleton<C>::Instance(1, 1.1);
	Singleton<A>::DestroyInstance();
	Singleton<B>::DestroyInstance();
	Singleton<C>::DestroyInstance();
	f(1, 2, "hello world!!!");
	std::cout << "==========================================" << std::endl;
	
	Events<std::function<void(int, int)> > myevent;
	std::vector<int> keyVec;
	
	keyVec.emplace_back(myevent.Connect(print));
	stA t;
	auto lambdaKey = myevent.Connect([&t](int a, int b) { 
		t.a = a; t.b = b; 
		std::cout << a << ", " << b << std::endl;
	});

	keyVec.emplace_back(lambdaKey);

	std::function<void(int, int)> f = std::bind(&stA::print, &t, std::placeholders::_1, std::placeholders::_2);
	keyVec.emplace_back(myevent.Connect(f));

	int a = 1;
	int b = 2;
	myevent.Notify(a, b);

	for (auto &it : keyVec) {
		std::cout << it << std::endl;
		myevent.Disconnect(it);
	}


	getchar();
	return 0;
}