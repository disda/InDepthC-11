#pragma once

class NonCopyable
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;
public:
	NonCopyable(const NonCopyable&) = delete;//禁止复制拷贝
	NonCopyable& operator = (const NonCopyable&) = delete; //禁止复制拷贝
};

