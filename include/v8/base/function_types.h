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

namespace v8 { namespace base {

template<typename T>
struct is_function_type {
private :
    typedef char    first_t;
    typedef struct {
        char a[2];
    } second_t;


    template<typename U> static first_t      test_fn(...);
    template<typename U> static second_t     test_fn(U (*)[1]);

public :
    enum {
        Yes = (sizeof(is_function_type<T>::test_fn<T>(nullptr))
               == sizeof(first_t)),
        No = !Yes
    };
};

template<typename T>
struct is_function_type<T&> {
    enum {
        Yes = 0,
        No = !Yes
    };
};

template<typename T>
struct is_function_type<const T&> {
    enum {
        Yes = 0,
        No = !Yes
    };
};

template<>
struct is_function_type<void> {
    enum {
        Yes = 0,
        No = !Yes
    };
};

template<>
struct is_function_type<void const> {
    enum {
        Yes = 0,
        No = !Yes
    };
};

template<>
struct is_function_type<void volatile> {
    enum {
        Yes = 0,
        No = !Yes
    };
};

template<>
struct is_function_type<void const volatile> {
    enum {
        Yes = 0,
        No = !Yes
    };
};

} // namespace base
} // namespace v8
