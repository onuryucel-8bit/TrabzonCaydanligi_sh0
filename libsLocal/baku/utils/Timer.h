#pragma once

#include <chrono>

namespace uti
{
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	using NanoSeconds = std::chrono::nanoseconds;
	using Seconds = std::chrono::seconds;
	using MilliSeconds = std::chrono::milliseconds;


	class Timer
	{
	public:
		Timer();
		~Timer();

		float getElapsedTime_milliseconds() const;
		float getElapsedTime_seconds() const;
		float getElapsedTime_nanoSeconds() const;

		void start();


	private:
		 TimePoint start_point;

	};

	inline float Timer::getElapsedTime_nanoSeconds() const
	{
		return 0;
	}

	inline float Timer::getElapsedTime_seconds() const
	{
		auto end_point = Clock::now();
		std::chrono::duration<float> dt  = end_point - start_point;
		return dt.count();
	}

	inline float Timer::getElapsedTime_milliseconds() const
	{
		return 0;
	}

	inline void Timer::start()
	{
		start_point = Clock::now();
	}

	inline Timer::Timer()
	{
		start();
	}

	inline Timer::~Timer()
	{
	}
}