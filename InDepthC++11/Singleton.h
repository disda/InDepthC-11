#pragma once
/*
1. C++11 ���Ƶ���ģʽ
	�����˿ɱ����ģ��
*/
template <typename T>
class Singleton {
public:
	//û��ʹ��C++�ɱ亯�������Ļ�������Ҫ�ظ����岻ͬ���Instance
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
			//����ת��
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