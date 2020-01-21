#pragma once
/*
1. C++11 改善单例模式
	增加了可变参数模板
*/
template <typename T>
class Singleton {
public:
	//没有使用C++可变函数参数的化，则需要重复定义不同类的Instance
#if 0
	static T* Instance() {
		if (m_pInstance == nullptr) {
			m_pInstance = new T();
		}
		return m_pInstance;
	}
	template <typename T0>
	static T* Instance(T0 arg0) {
		if (m_pInstance == nullptr) {
			m_pInstance = new T(arg0);
		}
		return m_pInstance;
	}
#endif

	template<typename... Args>
	static T* Instance(Args&&... args) {
		if (m_pInstance == nullptr) {
			//完美转发
			m_pInstance = new T(std::forward<Args>(args)...);
		}
		return m_pInstance;
	}

	static void Singleton<T>::DestroyInstance() {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
private:
	Singleton(void) {};
	virtual ~Singleton(void) {};
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);

	static T* GetInstance() {
		if (m_pInstance == nullptr) {
			throw std::logic_error("the instance is not init,please initialize the instance first");
		}
		return m_pInstance;
	}


private:
	static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance = nullptr;