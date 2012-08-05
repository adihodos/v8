#pragma once

#include <windows.h>
#include <cstdint>
#include <v8/base/compiler_quirks.h>

namespace v8 { namespace base {

/**
 * A simple timer, built around the timeGetTime() api function.
 */
template<typename real_t>
class basic_timer {
private :
    DWORD   t0_;
    DWORD   t1_;

public :
    basic_timer() : t0_(0), t1_(1) {}

    void start() {
        t0_ = ::timeGetTime();
    }

    void stop() {
        t1_ = timeGetTime();
    }

    /**
     * Gets the elapsed time between the reset() and stop() calls.
     *
     * \return  The time interval, in milliseconds.
     */
    real_t get_delta_ms() const {
        return (real_t(t1_) - real_t(t0_));
    }

    real_t tick() {
        stop();
        real_t delta = get_delta_ms();
        t0_ = t1_;
        return delta;
    }
};

/**
 * High resolution timer, using QueryPerformanceCounter() api function.
 */
template<typename real_t>
class high_resolution_timer {
private :
    real_t	        perf_multiplier_;
    int64_t	        start_;
    int64_t	        end_;

public :
    high_resolution_timer() : perf_multiplier_(0), start_(0), end_(0) {
        int64_t perf_counts_per_second = 0;
        ::QueryPerformanceFrequency(
            reinterpret_cast<LARGE_INTEGER*>(&perf_counts_per_second));
        perf_counts_per_second /= 1000;
        perf_multiplier_ = 1 / (real_t) perf_counts_per_second;
    }

    void start() {
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&start_));
    }

    void stop() {
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&end_));
    }

    /**
     * Gets the elapsed time between the reset() and stop() calls.
     *
     * \return  The time interval, <b>in milliseconds</b>.
     */
    real_t get_delta_ms() const {
        return real_t(end_ - start_) * perf_multiplier_;
    }

    /**
     * \brief Returns the number of elapsed milliseconds from the previous call
     *  of start()/tick(). Also updates the start time to match end time,
     * so that time intervals are continuous.
     */
    real_t tick() {
        stop();
        real_t delta = get_delta_ms();
        start_ = end_;
        return delta;
    }
};

/**
 * Helper class that will automatically reset a timer, when going out of scope.
 */
template<typename timer_type>
class auto_timer_reset {
private :
    NO_CC_ASSIGN(auto_timer_reset);
    timer_type&	timer_;	

public :
    auto_timer_reset(timer_type& timer) : timer_(timer) {}

    ~auto_timer_reset() {
        timer_.reset();
    }	
};

} // namespace base
} // namespace v8
