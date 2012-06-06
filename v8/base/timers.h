#pragma once

#include <windows.h>

#include "v8/base/misc.h"

namespace v8 { namespace base {

/**
 * A simple timer, built around the timeGetTime() api function.
 */
template<typename real_t>
class basic_timer {
private :
    DWORD start_;
    DWORD end_;

public :
    basic_timer() { reset(); }

    void reset() {
        start_ = timeGetTime();
    }

    void stop() {
        end_ = timeGetTime();
    }

    /**
     * Gets the elapsed time between the reset() and stop() calls.
     *
     * \return  The time interval, in milliseconds.
     */
    real_t get_delta_ms() const {
        return (real_t(end_) - real_t(start_));
    }

    real_t get_delta_ms_and_reset() {
        const real_t delta = get_delta_ms();
        reset();
        return delta;
    }
};

/**
 * High resolution timer, using QueryPerformanceCounter() api function.
 */
template<typename real_t>
class high_resolution_timer {
private :
    LARGE_INTEGER	perf_frequency_;
    LARGE_INTEGER	start_;
    LARGE_INTEGER	end_;

public :
    high_resolution_timer() {
        ::QueryPerformanceFrequency(&perf_frequency_);
        reset();
    }

    void reset() {
        ::QueryPerformanceCounter(&start_);
    }

    void stop() {
        ::QueryPerformanceCounter(&end_);
    }

    /**
     * Gets the elapsed time between the reset() and stop() calls.
     *
     * \return  The time interval, in milliseconds.
     */
    real_t get_delta_ms() const {
        return real_t(end_.QuadPart - start_.QuadPart) 
                / (real_t(perf_frequency_.QuadPart * 1000));
    }

    real_t get_delta_ms_and_reset() {
        const real_t delta = get_delta_ms();
        reset();
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
