#pragma once

/*
1. C++ 11中线程用法
线程同步的互斥量，用于线程通信的条件变量，线程安全的原子变量，call_once用法，用户异步操作的future,promise和task,以及异步操作函数async
join会阻塞线程，直到线程执行结束，如果有返回值，将被忽略

1.获取线程id，设备cpu核心数
2. 线程休眠
3. 互斥量是一种同步原语，是一种线程同步手段，用来保护多线程同时访问的共享数据。
std::mutex:独占的互斥量，不能递归使用
std::timed_mutex：带有超时的独占互斥量，不能递归使用
std::recursive_mutex：递归互斥量，不带超时功能
std::recursive_timed_mutex：带有超时功能的递归互斥量
*/
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex g_lock;

void funcTextMutex() {
	g_lock.lock();
	std::cout << "entered thread" << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;
	g_lock.unlock();
}

