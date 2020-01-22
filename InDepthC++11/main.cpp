#include <iostream>
#include <vector>

#include "Singleton.h"
#include "Observer.h"
#include "Vistor.h"
#include "SimpleCommand.h"
#include "ObjectPool.h"

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


//void TestVisitor() {
//	ConcreteVisitor v;
//	std::unique_ptr<Element> emt1(new ConcreteElement1());
//	std::unique_ptr<Element> emt2(new ConcreteElement2());
//
//	emt1->Accept(v);
//	emt2->Accept(v);
//}

void TestVisitor1() {
	PrintVisitor vis;
	stVA a;
	stVB b;
	a.Val = 8.90;
	b.Val = 8;
	Base* base = &a;
	base->Accept(vis);
	base = &b;
	base->Accept(vis);
}


int add_one(int n) {
	return n + 1;
}

void TestWrap() {
	CommCommand<int> cmd;
	cmd.Wrap(add_one, 0);
	cmd.Wrap(
		[](int n) {
		return n + 1;
	}, 1);

	//cmd.Wrap(bloop);
	//
	//cmd.Wrap(bloop, 4);

	STA t = { 10 };
	int x = 3;

	cmd.Wrap(&STA::triple0, &t);
	cmd.Wrap(&STA::triple, &t, x);
	cmd.Wrap(&STA::triple, &t, 3);

	cmd.Wrap(&STA::triple2, &t, 3);

	auto r = cmd.Excecute();

	CommCommand<> cmd1;
}



struct BigObject {
	BigObject() {}
	BigObject(int a) {}
	BigObject(const int& a, const int& b) {};
	void Print(const std::string& str) {
		std::cout << str << std::endl;
	}
};

void Print(shared_ptr<BigObject>p ,const std::string& str) {
	if (p != nullptr) {
		p->Print(str);
	}
}

void TestObjectPool() {
	ObjectPool<BigObject> pool;
	pool.Init(2);

	{
		auto p = pool.Get();
		Print(p, "p");
		auto p2 = pool.Get();
		Print(p2, "p2");
	}

	auto p = pool.Get();
	auto p2 = pool.Get();

	Print(p, "p");
	Print(p2, "p2");

	pool.Init(2, 1);
	auto p4 = pool.Get<int>();

	Print(p4, "p4");
	pool.Init(2, 1, 2);
	auto p5 = pool.Get<int,int>();
	Print(p5, "p5");
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
	std::cout << "==========================================" << std::endl;

	TestVisitor1();
	std::cout << "==========================================" << std::endl;

	TestWrap();
	std::cout << "==========================================" << std::endl;
	TestObjectPool();
	getchar();
	return 0;
}