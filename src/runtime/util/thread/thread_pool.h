#pragma once

#include <thread>
#include <vector>

namespace util
{

struct Job
{

};

class ThreadPool
{
public:
	ThreadPool(size_t poolSize);

private:
	std::vector<std::jthread> m_Workers;
};

}
