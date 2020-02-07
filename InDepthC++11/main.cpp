#include <iostream>
#include <vector>
#include <thread>
#include <memory>

#include "Singleton.h"
#include "Observer.h"
#include "Vistor.h"
#include "SimpleCommand.h"
#include "ObjectPool.h"
#include "CallOnce.h"
#include "testThread.h"
#include "Promise.h"
#include "DThreadPool.h"
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

/*
1. std::thread出了作用域将会析构，如果这时线程函数还没有执行完则会发生错误，因此，需要保证线程函数的
2. 线程不能复制，但可以移动 std::thread t(func); std::thread t1(std::move(t)); 线程移动后，线程对象不代表任何线程
3. 注意点：可以通过join方式阻塞线程的方式等待线程函数执行完，或者通过detach，让线程在后台执行完。
4. 可以把线程对象保存到一个容器中，以保证线程对象的生命周期。
*/

void func(int i, double d, const  std::string& s) {
	std::cout << i << ", " << d << ", " << s << std::endl;
}

void func1() {
	std::cout << "test thread" << std::endl;
}

std::vector<std::thread> g_list;
std::vector<std::shared_ptr<std::thread>> g_list2;


void CreateThread() {
	std::thread t(func1);
	g_list.push_back(std::move(t));
	g_list2.push_back(std::make_shared<std::thread>(func1));
}

//测试创建线程，并阻塞等待线程
#if 0
int main() {
	//可变函数参数模板
	std::thread t(func, 1, 2, "test");
	t.join();

	CreateThread();
	for (auto& thread : g_list) {
		thread.join();
	}

	for (auto& thread : g_list2) {
		thread->join();
	}
	getchar();
	return 0;
}
#endif



//设计模式的相关代码
#if 0
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
#endif

#if 0
int main() {
	std::thread t1(funcTextMutex);
	std::thread t2(funcTextMutex);
	std::thread t3(funcTextMutex);

	t1.join();
	t2.join();
	t3.join();

	std::thread t4(work);
	std::thread t5(work);

	t4.join();
	t5.join();
	getchar();
}
#endif

#if 0
//test std::call_once
int main() {
	std::thread t1(do_once);
	std::thread t2(do_once);
	std::thread t3(do_once);

	t1.join();
	t2.join();
	t3.join();
	getchar();
}
#endif

#if 0
int main() {
	TestPromise();
	std::cout << "================================================================" << std::endl;
	TestAsync();
	getchar();
}

#endif

#if 0
int main() {
	TestThreadPool();
}
#endif

using namespace std;
template<typename T>
void PrintT(T& t) {
	cout << "lvalue" << endl;
}

template<typename T>
void PrintT(T &&t) {
	cout << "right value" << endl;
}

template<typename T>
void TestForward(T &&v) {
	PrintT(v);
	PrintT(std::forward<T>(v));
	PrintT(std::move(v));
}

#if 0
int main() {
	TestForward(1);
	int x = 1;
	TestForward(x);
	TestForward(std::forward<int>(x));
	getchar();
}
#endif


//test emplace_back

#include <vector>
#include <map>
#include <string>
#include <iostream>
using namespace std;

struct Complicated {
	int year;
	double country;
	std::string name;

	Complicated(int a, double b, string c) :year(a), country(b), name(c) {
		cout << "is constructed" << endl;
	}

	Complicated(const Complicated& other) :year(other.year), country(other.country), name(std::move(other.name)) {
		cout << "is move" << endl;
	}
};

int main() {
	std::map<int, Complicated> m;
	int aInt = 4;
	double  aDouble = 1.0;
	std::string aString = "C++";
	cout << "--insert--" << endl;

	m.insert(std::make_pair(4, Complicated(aInt, aDouble, aString)));
	cout << "---emplace---" << endl;
	m.emplace(4, Complicated(aInt, aDouble, aString));
	cout << "---emplace_back---" << endl;
	vector<Complicated> v;
	v.emplace_back(aInt, aDouble, aString);

	cout << "---push_back---" << endl;
	v.push_back(Complicated(aInt, aDouble, aString));
	getchar();
}