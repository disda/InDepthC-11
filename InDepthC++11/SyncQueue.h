#pragma once

#include <mutex>
#include <thread>
#include <condition_variable>

template <typename T>
class SyncQueue {
	bool _isFull() const {
		return m_queue.size() == m_maxSize;
	}

	bool _isEmpty() const {
		return m_queue.empty();
	}
public:
	SyncQueue(int maxSize) :m_maxSize(maxSize) {
	}

	void Put(const T& x) {
		std::lock_guard<std::mutex> locker(m_mutex);
		while (_isFull()) {
			std::cout << "缓冲区已已满，需要等待" << std::endl;
			m_notFull.wait(m_mutex);
		}
		m_queue.push_back(x);
		m_notEmpty.notify_one();
	}

	void Take(T& x) {
		std::lock_guard<std::mutex> locker(m_mutex);
		while (_isEmpty()) {
			std::cout << "缓冲区空了，需要等待" << std::endl;
			m_notEmpty.wait(m_mutex);
		}

		x = m_queue.front();
		m_queue.pop_front();
		m_notFull.notify_one();
	}

	bool Empty() {
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.empty();
	}

	bool Full() {
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}

	size_t Size() {
		std::lock_guard<std::mutex> locker(m_mutex);
		return m_queue.size();
	}

	int Count() {
		return m_queue.size();
	}
private: 
	std::list<T> m_queue;
	std::mutex m_mutex;
	std::condition_variable_any m_notEmpty;
	std::condition_variable_any m_notFull;
	int m_maxSize;
};