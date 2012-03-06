#pragma once

#include "gfx_misc.h"

namespace gfx {

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

    static color from_u32_rbga(unsigned int u32color) {
        unsigned char red = u32color & (static_cast<unsigned int>(0xFF) << 24);
        unsigned char green = u32color & (static_cast<unsigned int>(0xFF) << 16);
        unsigned char blue = u32color & (static_cast<unsigned int>(0xFF) << 8);
        unsigned char alpha = u32color & static_cast<unsigned int>(0xFF);
        return color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static color from_u32_bgra(unsigned int u32bgra) {
        unsigned char blue = u32bgra & (static_cast<unsigned int>(0xFF) << 24);
        unsigned char green = u32bgra & (static_cast<unsigned int>(0xFF) << 16);
        unsigned char red = u32bgra & (static_cast<unsigned int>(0xFF) << 8);
        unsigned char alpha = u32bgra & static_cast<unsigned int>(0xFF);
        return color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    static color from_u32_argb(unsigned int u32argb) {
        unsigned char alpha = u32argb & (static_cast<unsigned int>(0xFF) << 24);
        unsigned char red = u32argb & (static_cast<unsigned int>(0xFF) << 16);
        unsigned char green = u32argb & (static_cast<unsigned int>(0xFF << 8));
        unsigned char blue = u32argb & (static_cast<unsigned int>(0xFF));
        return color(
            static_cast<float>(red) / 255.0f, 
            static_cast<float>(green) / 255.0f, 
            static_cast<float>(blue) / 255.0f, 
            static_cast<float>(alpha) / 255.0f
            );
    }

    unsigned int to_uint32_rgba() const {
        unsigned int red = static_cast<unsigned int>(255 * r_) << 24;
        unsigned int green = static_cast<unsigned int>(255 * g_) << 16;
        unsigned int blue = static_cast<unsigned int>(255 * b_) << 8;
        unsigned int alpha = static_cast<unsigned int>(255 * a_);
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

} // namespace gfx