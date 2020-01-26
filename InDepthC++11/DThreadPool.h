#pragma once

#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "DSyncQueue.h"

using Task = std::function<void()>;

const int MaxTaskCount = 100;
class ThreadPool {
public:
	ThreadPool(int numThreads = std::thread::hardware_concurrency()) :m_queue(MaxTaskCount) {
		Start(numThreads);
	}

	~ThreadPool() {
		Stop();
	}

	void Stop() {
		std::call_once(m_flag, [this] {
			StopThreadGroup();
		});
	}

	void AddTask(Task &&task) {
		m_queue.Put(std::forward<Task>(task));
	}

	void AddTask(const Task& task) { 
		m_queue.Put(task);
	}

	void Start(int numThreads) {
		m_running = true;
		for (int i = 0; i < numThreads; i++) { 
			m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
		}
	}
private:
	void RunInThread() {
		while (m_running) {
			std::list<Task> list;
			m_queue.Take(list);
			for (auto& task : list) {
				if (!m_running) {
					return;
				}
				task();
			}
		}
	}

	void StopThreadGroup() {
		m_queue.Stop();
		m_running = false;
		for (auto thread : m_threadgroup) {
			if (thread) {
				thread->join();
			}
		}
		m_threadgroup.clear();
	}
private:
	std::list<std::shared_ptr<std::thread>> m_threadgroup;
	DSyncQueue<Task> m_queue;
	atomic<bool> m_running;
	std::once_flag m_flag;
};


void TestThreadPool() {
	ThreadPool pool;
	pool.Start(2);
	std::thread thd1([&pool]() {
		for (int i = 0; i < 10; i++) {
			auto thdId = this_thread::get_id();
			pool.AddTask([thdId]() {
				cout << "同步线程1的线程ID:" << thdId << endl;
			});
		}
	});

	std::thread thd2([&pool]() {
		for (int i = 0; i < 10; i++) {
			auto thdId = this_thread::get_id();
			pool.AddTask([thdId]() {
				cout << "同步线程2的线程ID:" << thdId << endl;
			});
		}
	});

	this_thread::sleep_for(std::chrono::seconds(2));
	getchar();
	pool.Stop();
	thd1.join();
	thd2.join();
}