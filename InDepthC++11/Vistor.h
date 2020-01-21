#pragma once

/*
1. �������ݵ���Ϊ���ڲ��޸������������£������µĲ�����ɫ��ְ��
*/

#include <iostream>
#include <memory>
using namespace std;

//struct ConcreteElement1;
//struct ConcreteElement2;
//
//template <typename... Types>
//struct Visitor;
//
//template<typename T,typename... Types>
//struct Visitor : Visitor<Types...> {
//	//virtual ~Visitor() {}
//	//virtual void Visit(ConcreteElement1* element) = 0;
//	//virtual void Visit(ConcreteElement2* element) = 0;
//	using Visitor<Types...>::Visit;
//	virtual void Visit(const T&) = 0;
//};
//
//template<typename T>
//struct Visitor<T> {
//	virtual void Visitor(const T&) = 0;
//};

template<typename... Types>
struct Visitor;

template<typename T, typename... Types>
struct Visitor<T, Types...> : Visitor<Types...>
{
	using Visitor<Types...>::Visit;//ͨ��using�������ػ����visitͬ������
	virtual void Visit(const T&) = 0;
};

template<typename T>
struct Visitor<T>
{
	virtual void Visit(const T&) = 0;
};


//struct Element {
//	virtual ~Element() {}
//	virtual void Accept(Visitor& visitor) = 0;
//};
//
////���������
//struct ConcreteVisitor : public Visitor {
//	//�����߷��ʲ�ͬ�ı���������Ϊ��һ��
//	void Visit(ConcreteElement1* element) {
//		std::cout << "Visit ConcreteElement1" << endl;
//	}
//
//	void Visit(ConcreteElement2* element) {
//		std::cout << "Visit ConcreteElement2" << endl;
//	}
//};
//
//
//struct ConcreteElement1 : public Element {
//	void Accept(Visitor& visitor) { 
//		visitor.Visit(this);
//	}
//};
//
//struct ConcreteElement2 : public Element {
//	void Accept(Visitor& visitor) {
//		visitor.Visit(this);
//	}
//};


//�ɱ�ģ������Ľ�


struct stVA;
struct stVB;

struct Base {
	typedef Visitor<stVA, stVB> MytVisitor;
	virtual void Accept(MytVisitor&) = 0;
};

struct stVA : Base {
	double Val;
	void Accept(Base::MytVisitor& v) {
		v.Visit(*this);
	}
};

struct stVB : Base {
	int Val;
	void Accept(Base::MytVisitor& v) {
		v.Visit(*this);
	}
};

struct PrintVisitor : Base::MytVisitor {
	void Visit(const stVA& a) {
		std::cout << "from stA:" << a.Val << std::endl;
	}

	void Visit(const stVB& b) {
		std::cout << "from stB:" << b.Val << std::endl;
	}
};