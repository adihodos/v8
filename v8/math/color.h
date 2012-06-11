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

#include "v8/base/compiler_warnings.h"

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

    MSVC_DISABLE_WARNING_BLOCK_BEGIN(4201)
    union {
        struct {
            float r_; /*!<< Red component intensity */
            float g_; /*!<< Green component intensity */
            float b_; /*!<< Blue component intensity */
            float a_; /*!<< Alpha component (opacity) */
        };
        float components_[4];
    };
    MSVC_DISABLE_WARNING_BLOCK_END(4201)

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

    static const color AliceBlue;
    static const color AntiqueWhite;
    static const color Aqua;
    static const color Aquamarine;
    static const color Azure;
    static const color Beige;
    static const color Bisque;
    static const color Black;
    static const color BlanchedAlmond;
    static const color Blue;
    static const color BlueViolet;
    static const color Brown;
    static const color BurlyWood;
    static const color CadetBlue;
    static const color Chartreuse;
    static const color Chocolate;
    static const color Coral;
    static const color CornflowerBlue;
    static const color Cornsilk;
    static const color Crimson;
    static const color Cyan;
    static const color DarkBlue;
    static const color DarkCyan;
    static const color DarkGoldenRod;
    static const color DarkGray;
    static const color DarkGrey;
    static const color DarkGreen;
    static const color DarkKhaki;
    static const color DarkMagenta;
    static const color DarkOliveGreen;
    static const color Darkorange;
    static const color DarkOrchid;
    static const color DarkRed;
    static const color DarkSalmon;
    static const color DarkSeaGreen;
    static const color DarkSlateBlue;
    static const color DarkSlateGray;
    static const color DarkSlateGrey;
    static const color DarkTurquoise;
    static const color DarkViolet;
    static const color DeepPink;
    static const color DeepSkyBlue;
    static const color DimGray;
    static const color DimGrey;
    static const color DodgerBlue;
    static const color FireBrick;
    static const color FloralWhite;
    static const color ForestGreen;
    static const color Fuchsia;
    static const color Gainsboro;
    static const color GhostWhite;
    static const color Gold;
    static const color GoldenRod;
    static const color Gray;
    static const color Grey;
    static const color Green;
    static const color GreenYellow;
    static const color HoneyDew;
    static const color HotPink;
    static const color IndianRed;
    static const color Indigo;
    static const color Ivory;
    static const color Khaki;
    static const color Lavender;
    static const color LavenderBlush;
    static const color LawnGreen;
    static const color LemonChiffon;
    static const color LightBlue;
    static const color LightCoral;
    static const color LightCyan;
    static const color LightGoldenRodYellow;
    static const color LightGray;
    static const color LightGrey;
    static const color LightGreen;
    static const color LightPink;
    static const color LightSalmon;
    static const color LightSeaGreen;
    static const color LightSkyBlue;
    static const color LightSlateGray;
    static const color LightSlateGrey;
    static const color LightSteelBlue;
    static const color LightYellow;
    static const color Lime;
    static const color LimeGreen;
    static const color Linen;
    static const color Magenta;
    static const color Maroon;
    static const color MediumAquaMarine;
    static const color MediumBlue;
    static const color MediumOrchid;
    static const color MediumPurple;
    static const color MediumSeaGreen;
    static const color MediumSlateBlue;
    static const color MediumSpringGreen;
    static const color MediumTurquoise;
    static const color MediumVioletRed;
    static const color MidnightBlue;
    static const color MintCream;
    static const color MistyRose;
    static const color Moccasin;
    static const color NavajoWhite;
    static const color Navy;
    static const color OldLace;
    static const color Olive;
    static const color OliveDrab;
    static const color Orange;
    static const color OrangeRed;
    static const color Orchid;
    static const color PaleGoldenRod;
    static const color PaleGreen;
    static const color PaleTurquoise;
    static const color PaleVioletRed;
    static const color PapayaWhip;
    static const color PeachPuff;
    static const color Peru;
    static const color Pink;
    static const color Plum;
    static const color PowderBlue;
    static const color Purple;
    static const color Red;
    static const color RosyBrown;
    static const color RoyalBlue;
    static const color SaddleBrown;
    static const color Salmon;
    static const color SandyBrown;
    static const color SeaGreen;
    static const color SeaShell;
    static const color Sienna;
    static const color Silver;
    static const color SkyBlue;
    static const color SlateBlue;
    static const color SlateGray;
    static const color SlateGrey;
    static const color Snow;
    static const color SpringGreen;
    static const color SteelBlue;
    static const color Tan;
    static const color Teal;
    static const color Thistle;
    static const color Tomato;
    static const color Turquoise;
    static const color Violet;
    static const color Wheat;
    static const color White;
    static const color WhiteSmoke;
    static const color Yellow;
    static const color YellowGreen;
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
