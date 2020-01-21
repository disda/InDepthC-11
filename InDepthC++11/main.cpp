#include <iostream>
#include "Singleton.h"

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

int main() {
	Singleton<A>::Instance();
	Singleton<B>::Instance(1);
	Singleton<C>::Instance(1, 1.1);
	Singleton<A>::DestroyInstance();
	Singleton<B>::DestroyInstance();
	Singleton<C>::DestroyInstance();

	getchar();
	return 0;
}