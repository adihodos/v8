/*
 * gfx_vector.cc
 *
 *  Created on: Oct 5, 2011
 *      Author: adi.hodos
 */

#include "pch_hdr.h"
#include "vector4.h"

const gfx::vector4 gfx::vector4::null(0.0f, 0.0f, 0.0f, 0.0f);

const gfx::vector4 gfx::vector4::unit(1.0f, 1.0f, 1.0f, 0.0f);

const gfx::vector4 gfx::vector4::unit_x(1.0f, 0.0f, 0.0f, 0.0f);

const gfx::vector4 gfx::vector4::unit_y(0.0f, 1.0f, 0.0f, 0.0f);

const gfx::vector4 gfx::vector4::unit_z(0.0f, 0.0f, 1.0f, 0.0f);

void
gfx::mgs_ortho_normalize(
		const std::vector<gfx::vector4>& in,
		std::vector<gfx::vector4>& out
		)
{
	size_t vsize = in.size();
	for (size_t i = 0; i < vsize; ++i) {
		vector4 current_vec(in[i]);
		for (size_t j = 0; j < i; ++j) {
			current_vec = current_vec - projection(in[i], out[j]);
		}
		out.push_back(current_vec.normalize());
	}
}