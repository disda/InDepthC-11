#pragma once

/*
std::atomic<t>
c++11内置整型的原子变量，可以更方便地使用原子变量
*/

#include <atomic>

struct AtomicCounter {
	std::atomic<int> value;

	void increment() {
		++value;
	}


	void decrement() { 
		--value;
	}

	int get() {
		return value.load();
	}
};