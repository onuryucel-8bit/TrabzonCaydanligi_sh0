/**
 * A simple timer library, with a thread-safe sample accumulator.
 *
 * Basic usage:
 *    using TinyTimer::Timer;
 *    Timer timer;
 *    // .. do something
 *    cout << "Something took " << timer.elapsed() << " seconds" << endl;
 *
 * One can also consolidate several timings to measure standard deviation:
 *    PerformanceCounter perf;
 *    for (...) {
 *        Timer timer;
 *        // .. do something
 *        perf.add_sample(timer);
 *    }
 *    cout << "Something took " << perf.average() << "±" << perf.stddev() << " seconds" << endl;
 *
 * To make it easier to measure performances at any point of the code, a
 * global pool of counters is defined *statically* (there is no runtime cost
 * in accessing the counter):
 *    // Somewhere deep in your code:
 *    Timer timer;
 *    // .. do something
 *    PERF(42).add_sample(timer);
 *
 *    // Somewhere else, for instance at the end of main():
 *    cout << "Something took " << PERF(42).average() << "±" << PERF(42).stddev() << " seconds" << endl;
 *
 * If only the index "42" is used in the code then only one global PerformanceCounter
 * is defined. Counter index can only be int, or at least something that might be cast
 * into an int, so the recommended way of specifying it is by defining an enum:
 *    enum PerfCounterType {
 *        SomethingTime,
 *        SomethingElseTime,
 *    };
 *
 *    Timer timer;
 *    // .. do something
 *    PERF(SomethingTime).add_sample(timer);
 *
 *    timer.reset();
 *    // .. do something else
 *    PERF(SomethingElseTime).add_sample(timer);
 *
 *    cout << "Something:      " << PERF(SomethingTime) << endl;
 *    cout << "Something else: " << PERF(SomethingElseTime) << endl;
 *
 * Actually using enum values is more flexible because then you may mix keys
 * from different enums:
 *    enum PerfCounterType {
 *        SomethingTime = 0,
 *    };
 *    enum AnotherPerfCounterType {
 *        SomethingElseTime = 0, // even if they correspond to same value!
 *    };
 *    assert(PERF(SomethingTime) != PERF(SomethingElseTime))
 */

#pragma once

#include <chrono>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include <mutex>
#include <vector>

namespace TinyTimer {

    class Timer
    {
    public:
        Timer() noexcept {
            reset();
        }

        void reset() noexcept {
            begin = ::std::chrono::high_resolution_clock::now();
        }

        /**
         * elapsed time since either construction or last call to reset(), in nanoseconds
         */
        ::std::chrono::nanoseconds elapsedNanoseconds() const noexcept {
            auto end = ::std::chrono::high_resolution_clock::now();
            auto elapsed = ::std::chrono::duration_cast<::std::chrono::nanoseconds>(end - begin);
            return elapsed;
        }

        /**
         * elapsed time since either construction or last call to reset(), in seconds
         */
        double elapsed() const noexcept {
            return elapsedNanoseconds().count() * 1e-9;
        }

    private:
        ::std::chrono::time_point<::std::chrono::high_resolution_clock> begin;
    };

    /**
     * Accumulate time measurements, providing standard deviation
     */
    class PerformanceCounter
    {
    public:
        PerformanceCounter() noexcept {}

        PerformanceCounter(const PerformanceCounter& other) noexcept {
            std::lock_guard<std::mutex> lock(other.m_mutex);
            m_sample_count = other.m_sample_count;
            m_accumulated = other.m_accumulated;
            m_accumulated_sq = other.m_accumulated_sq;
            // NB: mutex is not copied!
        }

        PerformanceCounter(PerformanceCounter&& other) noexcept {
            std::lock_guard<std::mutex> lock(other.m_mutex);
            m_sample_count = other.m_sample_count;
            m_accumulated = other.m_accumulated;
            m_accumulated_sq = other.m_accumulated_sq;
            // NB: mutex is not moved!
        }

        void reset() noexcept {
            m_sample_count = 0;
            m_accumulated = 0;
            m_accumulated_sq = 0;
        }

        // Not thread safe
        int samples() const noexcept {
            return m_sample_count;
        }

        // Not thread safe
        double average() const noexcept {
            return m_sample_count == 0 ? 0 : m_accumulated / static_cast<double>(m_sample_count);
        }

        // Not thread safe
        double stddev() const noexcept {
            if (m_sample_count == 0) return 0;
            double avg = average();
            double var = m_accumulated_sq / static_cast<double>(m_sample_count) - avg * avg;
            return ::std::sqrt(::std::max(0.0, var));
        }

        void add_sample(const Timer& timer) noexcept {
            add_sample(timer.elapsed());
        }

        /**
         * Add a sample measure, in seconds
         */
        void add_sample(double elapsed) noexcept {
            std::lock_guard<std::mutex> lock(m_mutex);
            ++m_sample_count;
            m_accumulated += elapsed;
            m_accumulated_sq += elapsed * elapsed;
        }

        // Not thread safe
        ::std::string summary() const {
            ::std::ostringstream ss;
            ss << average() * 1e3 << "ms";
            ss << " (+-" << stddev() * 1e3 << "ms,";
            ss << " " << m_sample_count << " samples)";
            return ss.str();
        }

    private:
        unsigned int m_sample_count = 0;
        double m_accumulated = 0;
        double m_accumulated_sq = 0;
        mutable std::mutex m_mutex;
    };

    inline ::std::ostream& operator<<(::std::ostream& os, const PerformanceCounter& counter) {
        return os << "PerformanceCounter(" << counter.summary() << ")";
    }

    template<typename Enum, Enum key>
    struct counters {
        static PerformanceCounter& GetValue() {
            static PerformanceCounter value = PerformanceCounter();
            return value;
        }
    };

#define PERF(key) TinyTimer::counters<decltype(key), key>::GetValue()

    template<typename Enum, Enum key>
    struct counter_arrays {
        static std::vector<PerformanceCounter>& GetValue() {
            static std::vector<PerformanceCounter> value = {};
            return value;
        }
    };

#define PERF_ARRAY(key) TinyTimer::counter_arrays<decltype(key), key>::GetValue()

}