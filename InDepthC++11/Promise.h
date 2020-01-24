#pragma once


/*
1. C++11提供了异步操作的相关类，std::future,std::promise,std::package_task
2. std::package_task包装一个可调用的对象
3. future_status 有三种状态：Deferred,异步操作还没有开始，Ready异步操作已经完成，Timeout异步操作超时
4. std::promise: 同步工作线程和数据库线程
5. future被promise，package_task用来作为异步操作或者异步结果的连接通道，用std::furture和std::shared_future来调用异步调用结果

线程异步操作函数async
std::async比std::promise,std::packaged_task和std::thread更高一层，用来直接创建异步的task，异步任务返回的结果保存在futrue中，如果不关心future.wait即可
std::async的原型是async(std::launch::async | std::launch::deferred,f,args...),第一个参数是创建线程的策略，默认是立即创建线程
*/

#include <iostream>
#include <utility>
#include <future>
#include <thread>

int funcPromise(int x) {
	return x + 2;
}

void TestPromise() {
	std::packaged_task<int(int)> task(funcPromise);
	std::future<int> fut = task.get_future();

	std::thread(std::move(task), 2).detach();

	int value = fut.get();
	std::cout << "The result is " << value << std::endl;

	std::vector<std::shared_future<int>> v;
	std::shared_future<int> f = std::async(std::launch::async, [](int a, int b) {return a + b; }, 2, 3);

	v.push_back(f);
	std::cout << "The shared_futureresult is " << v[0].get() << std::endl;
	getchar();
}

void TestAsync() {
	std::future<int> f1 = std::async(std::launch::async, []() {
		return 8;
	});

	std::cout << f1.get() << std::endl;

	std::future<int> f2 = std::async(std::launch::async, []() {
		std::cout << 8 << std::endl;
		return 8;
	});
	f2.wait();

	std::future<int> future = std::async(std::launch::async, []() {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return 8;
	});

	std::future_status status;

	std::cout << "waiting..." << std::endl;

	do {
		status = future.wait_for(std::chrono::seconds(1));
		if (status == std::future_status::deferred) {
			std::cout << "deferred" << std::endl;
		} else if(status == std::future_status::timeout) {
			std::cout << "timeout" << std::endl;
		}
		else if (status == std::future_status::ready) {
			std::cout << "ready" << std::endl;
		}
	} while (status != std::future_status::ready);

	std::cout << "result is" << future.get() << std::endl;
}
