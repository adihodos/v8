//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "v8/base/handle_traits.h"
#include "v8/base/misc.h"

namespace v8 { namespace base {

/**
 * \brief RAII class for different OS handles.
 *        A handle is an "obfuscated pointer" to a resource allocated by a system
 *        (socket descriptors, file descriptors, etc...).
 */
template<typename management_policy>
class scoped_handle {
public :
    typedef management_policy                           mpolicy_t;
    typedef typename mpolicy_t::handle_t                handle_t;
    typedef typename mpolicy_t::handle_ptr_t            handle_ptr_t;
    typedef typename mpolicy_t::handle_ref_t            handle_ref_t;
    typedef typename mpolicy_t::handle_const_ref_t      handle_const_ref_t;
    typedef scoped_handle<management_policy>            self_t;

private :
    NO_CC_ASSIGN(scoped_handle);
    /*!< Owned handle */
    handle_t    handle_;

    struct helper_t {
        int member;
    };

    handle_t get() const {
        return handle_;
    }

    handle_ptr_t get_impl_ptr() {
        return &handle_;
    }

    handle_t release() {
        handle_t tmpHandle = handle_;
        handle_ = mpolicy_t::null_handle();
        return tmpHandle;
    }

    void reset(handle_t newValue) {
        if (handle_ != newValue) {
            mpolicy_t::dispose(handle_);
            handle_ = newValue;
        }
    }

    void swap(self_t& rhs) {
        std::swap(handle_, rhs.handle_);
    }

public :
    /**
     * \brief Default constructor. Initializes object with a non valid handle of
     * the corresponding type.
     */
    scoped_handle() : handle_(mpolicy_t::null_handle()) {}

    /**
     * \brief Initialize with an existing handle. The object assumes ownership
     * of the handle.
     */
    explicit scoped_handle(handle_t newHandle) : handle_(newHandle) {}

    scoped_handle(self_t&& right) {
        handle_ = right.release();
    }

    ~scoped_handle() {
        mpolicy_t::dispose(handle_);
    }

    operator int helper_t::*() const {
        return handle_ == mpolicy_t::null_handle() ? nullptr :
            &helper_t::member;
    }

    bool operator!() const {
        return handle_ == mpolicy_t::null_handle();
    }

    /**
     * \brief Explicit access to the raw handle owned by this object.
     */
    template<typename M>
    friend typename scoped_handle<M>::handle_t scoped_handle_get(
        const scoped_handle<M>& sh);

    /**
     * \brief Release ownership of the native handle to the caller.
     */
    template<typename M> 
    friend typename scoped_handle<M>::handle_t scoped_handle_release(
        scoped_handle<M>& sh);

    /**
     * \brief Reset the owned handle to a new value.
     * \remarks The old handle is destroyed before the assignment.
     */
    template<typename M>
    friend void scoped_handle_reset(
        scoped_handle<M>& sh, 
        typename scoped_handle<M>::handle_t newValue);

    /**
     * \brief Returns a pointer to the raw handle.
     */
    template<typename M>
    friend typename scoped_handle<M>::handle_ptr_t scoped_handle_get_impl(scoped_handle<M>& sh);

    /**
     * \brief Swaps contents with another object of this type.
     */
    template<typename M> 
    friend void swap(scoped_handle<M>& left, scoped_handle<M>& right);
    
    self_t& operator=(self_t&& right) {
        if (this != &right) {
            mpolicy_t::dispose(handle_);
            handle_ = right.release();
        }
        return *this;
    }
};

template<typename M>
inline typename scoped_handle<M>::handle_t scoped_handle_get(
    const scoped_handle<M>& sh) {
    return sh.get();
}

template<typename M> 
inline typename scoped_handle<M>::handle_t scoped_handle_release(
    scoped_handle<M>& sh) {
    return sh.release();
}

template<typename M>
void scoped_handle_reset(
    scoped_handle<M>& sh, 
    typename scoped_handle<M>::handle_t newValue = typename scoped_handle<M>::mpolicy_t::null_handle()) {
    sh.reset(newValue);
}

template<typename M>
typename scoped_handle<M>::handle_ptr_t scoped_handle_get_impl(
    scoped_handle<M>& sh) {
    return sh.get_impl_ptr();
}

template<typename M> 
void swap(scoped_handle<M>& left, scoped_handle<M>& right) {
    left.swap(right);
}

template<typename T>
inline bool operator==(const typename T::handle_t& left,
                       const scoped_handle<T>& right) {
    return left == scoped_handle_get(right);
}

template<typename T>
inline bool operator==(const scoped_handle<T>& left,
                       const typename T::handle_t& right) {
    return right == left;
}

template<typename T>
inline bool operator!=(const typename T::handle_t& left,
                       const scoped_handle<T>& right) {
    return !(left == right);
}

template<typename T>
inline bool operator!=(const scoped_handle<T>& left,
                       const typename T::handle_t& right) {
    return right != left;
}

} // namespace base
} // namespace v8
