#pragma once

#include <pdh.h>

#include "v8/base/compiler_quirks.h"

namespace v8 { namespace base {

class cpu_counter {
private :
    NO_CC_ASSIGN(cpu_counter);

    PDH_HQUERY      queryhandle_;
    PDH_HCOUNTER    counter_;
    bool            valid_;

public :
    cpu_counter() : valid_(false) {
        const wchar_t* const kPerfCntName = L"\\Processor(0)\\% Processor Time";
        valid_ = (::PdhOpenQueryW(nullptr, 0, &queryhandle_) == ERROR_SUCCESS)
                  && (::PdhAddCounterW(queryhandle_, kPerfCntName, 0, &counter_) 
                      == ERROR_SUCCESS);
    }

    ~cpu_counter() {
        if (valid_) {
            ::PdhRemoveCounter(counter_);
            ::PdhCloseQuery(queryhandle_);
        }
    }

    bool operator!() const {
        return !valid_;
    }

    double get_cpu_usage() const {
        double cpu_usage = 0.0;
        if (valid_) {
            PDH_FMT_COUNTERVALUE cnt_val = { 0 };
            if (::PdhCollectQueryData(queryhandle_) == ERROR_SUCCESS) {
                if (::PdhGetFormattedCounterValue(counter_, PDH_FMT_DOUBLE, 
                                                  nullptr, &cnt_val) == ERROR_SUCCESS) {
                    cpu_usage = cnt_val.doubleValue;
                } 
            }
        }
        return cpu_usage;
    }
};

} // namespace base
} // namespace v8
