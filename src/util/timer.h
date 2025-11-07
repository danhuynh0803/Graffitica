#include <chrono>
#include <print>
#include <ratio>

namespace util
{
class Timer
{
public:
	Timer(const std::string& name = "Timer")
		: m_Name(name)
	{
		m_Start = std::chrono::steady_clock::now();
	}

	~Timer()
	{
		m_End = std::chrono::steady_clock::now();
		std::print("{0}: {1}\n", m_Name, std::chrono::duration<double, std::milli>(m_End - m_Start));
	}

	void Reset()
	{
		m_Start = std::chrono::steady_clock::now();
	}

private:
	std::chrono::steady_clock::time_point m_Start;
	std::chrono::steady_clock::time_point m_End;
	std::string m_Name;
};

#define CAT(x, y) x ## y
#define PROFILER util::Timer CAT(timer, __LINE__)(__FUNCTION__)

}