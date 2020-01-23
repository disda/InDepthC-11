#pragma once

/*
1.call_once/once_flag的使用

std::call_once来保证函数在多线程环境中只能被调用一次，需要一个once_flag作为call_once的入参
*/

#include <iostream>
#include <thread>
#include <mutex>

std::once_flag flag;

void do_once() {
	std::call_once(flag, []() {
		std::cout << "Called once" << std::endl;
	});
}