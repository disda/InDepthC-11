InDepthC++11

主要为<<深入应用C++11-代码优化以及工程应用>> 自己按照书本敲的demo集合

### 第一章

#### 返回类型后置语法,auto和decltype的结合使用
在泛型编程中,通过参数运算来得到返回值类型
```c++
template <typename R,typename T,typename U>
R Add(T t,U u) {
    return r+u;
}

int a = 1;float b = 2.0;
auto c = Add<decltype(a+b)>(a,b);
```
普通函数返回值类型推导
```c++
template <typename T,typename U>
decltype(T() + U()) add(T t,U u) {
    return t + u;
}

template <typename T,typename U>
decltype((*(T*)0) + (*(U*)0)) add(T t,U u) {
    return t + u;
}
```
类型推导后置语法
```cpp
template <typename T,typename U> 
auto add(T t,U u) -> decltype(t+u) {
    return t + u;
}
```
```cpp
int& foo(int& i);
float foo(float);

template <typename T>
auto func(T& val)->decltype(foo(val)) {
    return foo(val);
}

```

#### 模板>>括号的该进
C++11标准中可以
```cpp
Foo<A<int>>
```

#### 模板别名
定义一个固定std::string为key的map,他可以映射到int或另一个std::string
用typedef需要这样
```cpp
template <typename Val>
struct str_map {
    typedef std::map<std::string,Val> type;
};

str_map<int>::type mapl;
```
使用C++11的标准可以
```c++
template <typename Val>
using str_map_t = std::map<std::string,Val>;
str_map_t<int> mapl;
```
using的别名语法覆盖覆盖了typedef的全部功能
```cpp
typedef unsigned int uint_t;
using uint_t = unsigned int;

typedef std::map<std::string,int> map_int_t;
using map_int_t = std::map<std::string,int>;
```

定义函数参数
```cpp
typedef void (*func_t)(int,int);
using func_t = void(*)(int,int);
```

#### 函数模板的默认模板参数
在C++98/03的标准中，类模板可以有默认模板参数
```cpp
template <typename T,typename U = int,U N = 0>
struct Foo {

};

//现在也支持函数默认模板参数
template <typename T = int>
void func(void) {

}
```

#### 列表初始化
```cpp
int a5 = {4};
int a6 {3}; //c++11中可以直接在变量名后面跟上初始化列表
```

#### 列表初始化细节
聚合类型定义
- 类型是一个普通的数组
- 类型是一个类且
无用户自定义的构造函数
a. 无私有或者保护的非静态数据成员
b. 无基类
c. 无虚函数
d. 不能有{}和=直接初始化的非静态成员函数

#### 初始化列表
任意长度的初始化列表
stl容器使用std::initializer_list这个类模板来完成此功能
```cpp
class FooVector {
    std::vector<int> content_;
public:
    FooVector(std::initializer_list<int> lsit) {
        for(auto it = list.begin();it != list.end();++it;) {
            content_.push_back(*it);

        }
    }
};

class FooMap{
    std::map<int,int> content_;
    using pair_t = std::map<int,int>::value_type;
public:
    FooMap(std::initializer_list<pair_t> list) {
        for(auto it = list.begin();it != list.end();++it) {
            content_.insert(*it);
        }
    }
};
```
std::initializer_list的细节
- 轻量级容器类型，内部定义了iterator等容器必须的概念
- 对于std::initializer_list<T>而言，它可以接受任意长度的初始化列表，但要求元素类型必须是同类型T
- 有三个接口：size(),begin(),end()
- 只能被整体初始化或赋值
- std::initializer_list非常高效，它的内部不负责保存初始化列表中的元素。

#### 防止类型收窄
int b = {1.1};//c++11中是错误的，初始化列表不允许这种转换发生

#### 基于范围的for循环
```cpp
#include <iostream>
#include <vector>

int main(void) {
    std::vector<int> arr = {1,2,3};
    for(auto n : arr) {
        std::cout << n << std::endl;
    }
    return 0;
}
```

#### 基于范围的for循环支持自定义类型
```cpp
namespace detail_range {
template <typename T>
class iterator {
public:
    using value_type = T;
    using size_type = size_t;
private:
    size_type cursor_;
    const value_type step_;
    value_type value_;
public:
    iterator(size_type cur_start,value_type begin_val,value_type step_val)
    :cursor_(cur_start)
    ,step_(step_val)
    ,value_(begin_val) {
        value_ += (step_ * cursor_);
    }

    value_type operator*() const {
        return value_;
    }

    bool operator!=(const iterator& rhs) const
        return  (cursor_ != mrhs.cursor_);
    }

    iterator& operator++(void) {
        value_ += step_;
        ++cursor_;
        return (*this);
    }
};
}
```

#### std::function和bind绑定器
c++可调用对象
- 是一个函数指针
- 是一个具有operator() 成员函数的类对象
- 是一个可被转换为函数指针的类对象
- 是一个类成员(函数)指针

```cpp
void func() {

}

struct Foo{
    void operator() (void) {

    }
};

struct Bar {
    using fr_t = void(*)(void);
    static void func(void) {

    }

    operator fr_t(void) {
        return func;
    }
};

struct A {
    int a_;
    void mem_func(void) {

    }
};

int main(void) {
    void(*func_ptr)(void) = &func;
    func_ptr();
    Foo foo;    //仿函数
    foo();

    Bar bar;    //可以转换为函数指针的类对象
    bar();

    void (A::*mem_func_ptr)(void);  //类成员函数指针
    int A::*mem_obj_ptr = &A::mem_obj_ptr = &A::a_;

    A aa;
    (aa.*mem_func_ptr)();
    aa.*mem_obj_ptr = 123;
    return;
}
```
C++11 通过提供std::function和std::bind统一了可调用对象的各种操作

#### 可调用对象包装器---std::function
std::function是可调用对象的包装器，它是一个类模板，可以容纳除了类成员之外的所有可调用对象。通过指定它的模板参数，它可以用统一的方式处理函数。

```cpp
#include <iostream>
#include <functional>

void func(void) {
    std::cout << __FUNCTION__ << std::endl;
}

class Foo {
public:
    static int foo_func(int a) {
        std::cout << __FUNCTION__ << "(" << a << ")" ->:";
        return a;
    }
};

class Bar {
public:
    int operator() (int a) {
        std::cout << __FUNCTION__ << "(" << a << ") ->: ";
        return a;
    }
};

int main(void) {
    std::function<void(void)> fr1 = func;
    fr1();

    std::function<int(int)> fr2 = Foo::foo_func;
    std::cout << fr2(123) << std::endl;

    Bar bar;
    fr2 = bar;
    std::cout << fr2(123) << std::endl;
    return 0;
}
```
回调函数示例
```cpp
#include <iostream>
#include <functional>

class A {
    std::function<void()> callback_;
public:
    A(const std::function<void()>& f):callback_(f){

    }
    void notify(void) {
        callback_();
    }
};

class Foo {
public:
    void operator() (void) {
        std::cout << __FUNCTION__ << std::endl;
    }
};

int main(){
    Foo foo;
    A aa(foo);
    aa.notify();
    return 0;
}
```
std::function还可以作为函数入参
```cpp
#include <iostream>
#include <functional>

void call_when_even(int x,const std::function<void(int)>& f) {
    if (!(x&1)) {
        f(x);
    }
}

void output(int x) {
    std::cout << x << " ";
}

int main() {
    for(int i=0;i<10; ++i) {
        call_when_even(i,output);
    }
    std::cout << std::endl;
    return 0;
}
```

#### std::bind绑定器
std::bind用来将可调用对象与其参数一起进行半丁，绑定后的结果可以通过std::function进行保存，并延迟调用到任何需要的地方
- 将可调用对象与其参数一起绑定成一个仿函数
- 将多元（参数个数为n，n > 1)可调用对象转化为一元或者（n-1)元可调用对象，即直接绑定部分参数

std::bind基本用法
```cpp
#include <iostream>
#include <functional>

void call_when_even(int x,const std::function<void(int)>& f) {
    if(!(x&1)) {
        f(x);
    }
}

void output(int x) {
    std::cout << x << " ";
}

void output_add_2(int x) {
    std::cout << x+2  << " ";
}

int main() {
    {
        auto fr = std::bind(output, std::placeholders::_1);
        for(int i = 0;i<10;++i) {
            call_when_even(i,fr);
        }
        std::cout << std::endl;
    }

    {
        auto fr = std::bind(output_add_2,std::placeholders::_1);
        for(int i =0;i<10;i++) {
            call_when_even(i,fr);
        }
        std::cout << std::endl;
    }
    return 0;
}
```
std::placeholders::_1是一个占位符

std::bind和std::function配合使用
```cpp
class A{
public:
    int i_ = 0;
    void output(int x,int y) {
        std::cout << x << " " << std::endl;
    }
};

int main() {
    A a;
    std::function<void(int,int)> fr = std::bind(&A::output,&a,std::placeholders::_1,std::placeholders::_2);
    fr(1,2);

    std::function<int&(void)> fr_i  = std::bind(&A::i_,&a);
    fr_i() = 123;
    std::cout << a.i_ << std::endl;
    //std::bind将A的成员i_和a绑定
}
```

复合多个函数
```cpp
using std::placeholders::_1;
auto f = std::bind(std::logical_and<bool>(),std::bind(std::greater<int>(),_1,5),std::bind(std::less_equal<int>(),_1,10));
int count = std::count_if(coll.begin(),coll.end(),f);
```

#### lambda表达式
mutable的lambda表达式
```cpp
int a = 0;
auto f1 = [=]{return a++;}; //这个错误 按值捕获的外部变量不能修改
auto f2 = [=] mutable {return a++;};
```

#### 声明式的编程风格，简洁的代码
利用真正的lambda替换掉仿函数
```cpp
std::vector<int> v = {1,2,3,4,5,6};
int even_count = 0;
for_each(v.begin(),v.end(),[&even_count](int val){
    if(!(val & 1)) {
        ++event_count;
    }
});

std::cout << "The number of even is "<< even_count << std::endl;
```

#### tupe元组
固定大小的不同类型值的集合
```cpp
tuple<constchar*,int> tp = make_tuple(sendPack,nSendSize);//构造一个tuple
```
等价于一个结构体
struct A{
    char* p;
    int len;
};

std::tie，创建一个元组的左值引用
获取元组的值
```cpp
const char* data = tp.get<0>();
int len = tp.get<1>();

std::tie(std::ignore,std::ignore,y) = tp;
```

