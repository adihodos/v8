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

namespace v8 { namespace math {

/*!
 *\brief Represents a 4 component (red, green, blue, alpha) 
 *       normalized color vector (128 bits).
 *\remarks Color operations (addition, substraction, 
 *         component-wise multiplication, scalar multiplication) can result
 *         in individual components having values out of the [0, 1] range, so
 *         some form of normalization should be used, to correct those situations.
 */
class color {
public :
    union {
        struct {
            float r_; /*!<< Red component intensity */
            float g_; /*!<< Green component intensity */
            float b_; /*!<< Blue component intensity */
            float a_; /*!<< Alpha component (opacity) */
        };

        float components_[4];
    };

    color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) 
        : r_(r), g_(g), b_(b), a_(a) {}

    static color from_u32_rgba(unsigned int u32color) {
        unsigned char red = (u32color >> 24) & 0xFF;
        unsigned char green = (u32color >> 16) & 0xFF;
        unsigned char blue = (u32color >> 8) & 0xFF;
        unsigned char alpha = u32color & 0xFF;
        return color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static color from_u32_bgra(unsigned int u32bgra) {
        unsigned char blue = (u32bgra >> 24) & 0xFF;
        unsigned char green = (u32bgra >> 16) & 0xFF;
        unsigned char red = (u32bgra >> 8) & 0xFF;
        unsigned char alpha = u32bgra & 0xFF;
        return color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static color from_u32_argb(unsigned int u32argb) {
        unsigned char alpha = (u32argb >> 24) & 0xFF;
        unsigned char red = (u32argb >> 16) & 0xFF;
        unsigned char green = (u32argb >> 8) & 0xFF;
        unsigned char blue = u32argb & 0xFF;
        return color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    unsigned int to_uint32_rgba() const {
        unsigned int red = static_cast<unsigned int>(ceil(255 * r_)) << 24;
        unsigned int green = static_cast<unsigned int>(ceil(255 * g_)) << 16;
        unsigned int blue = static_cast<unsigned int>(ceil(255 * b_)) << 8;
        unsigned int alpha = static_cast<unsigned int>(ceil(255 * a_));
        return red | green | blue | alpha;
    }

    color& operator+=(const color& rhs) {
        r_ += rhs.r_;
        g_ += rhs.g_;
        b_ += rhs.b_;
        a_ += rhs.a_;
        return *this;
    }

    color& operator-=(const color& rhs) {
        r_ -= rhs.r_;
        g_ -= rhs.g_;
        b_ -= rhs.b_;
        a_ -= rhs.a_;
        return *this;
    }

    color& operator*=(float k) {
        r_ *= k;
        g_ *= k;
        b_ *= k;
        a_ *= k;
        return *this;
    }

    /*!
     *\brief Performs a componentwise multiplication between the two colors.
     */
    color& operator*=(const color& other) {
        r_ *= other.r_;
        g_ *= other.g_;
        b_ *= other.b_;
        a_ *= other.a_;
        return *this;
    }

    color& operator/=(float scalar) {
        float k = 1.0f / scalar;
        r_ *= k;
        g_ *= k;
        b_ *= k;
        a_ *= k;
        return *this;
    }
};

inline
color
operator+(
    const color& lhs,
    const color& rhs
)
{
    color result(lhs);
    result += rhs;
    return result;
}

inline
color
operator-(
    const color& lhs,
    const color& rhs
)
{
    color result(lhs);
    result -= rhs;
    return result;
}

inline
color
operator*(
    const color& lhs,
    const color& rhs
    )
{
    color result(lhs);
    result *= rhs;
    return result;
}

inline
color
operator*(
    const color& lhs,
    float k
    )
{
    color result(lhs);
    result *= k;
    return result;
}

inline
color
operator*(
    float k,
    const color& rhs
    )
{
    return rhs * k;
}

inline
color
operator/(
    const color& lhs,
    float scalar
    )
{
    float inv = 1.0f / scalar;
    return lhs * inv;
}

} // namespace math
} // namespace v8