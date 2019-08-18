/*__                          __
 / /____ ___  ___ ___  ____  / /
/ __/ -_) _ \(_-</ _ \/ __/ / _ \
\__/\__/_//_/___/\___/_/ (_)_//_/
 *
 * Drew O'Malley
 */
#ifndef TENSOR_H
#define TENSOR_H

#include <qmisc/macros.h>
/* types */
/* Transpose types, these are identical to their regular vector types but are
 * conceptually different, being row vectors as oppose to column vector
 * equivalents. This allows you to differentiate them for function signatures
 * and the like, allowing function signatures like float (*)(v2T_t, v2_t) */

typedef union {
	struct { float x, y; };
	struct { float r, g; };
	struct { float u, v; };
float arr[2];
} v2T_t; qsassert(sizeof(v2T_t) == 2*sizeof(float), "v2 != 8bytes");

typedef union {
	struct { float __1, y; };
	struct { v2T_t xy; float z; };
	struct { float x; v2T_t yz; };
	struct { float __2, g; };
	struct { v2T_t rg; float b; };
	struct { float r; v2T_t gb; };
	struct { float __3, v; };
	struct { v2T_t uv; float w; };
	struct { float u; v2T_t vw; };
	float arr[3];
} v3T_t; qsassert(sizeof(v3T_t) == 3*sizeof(float), "v3 != 12bytes");

typedef union {
	struct { float x, y, z, W; };
	struct { v2T_t xy, zW; };
	struct { float __1; v2T_t yz; };
	struct { v3T_t xyz; };
	struct { float __2; v3T_t yzW; };
	struct { float r, g, b, a; };
	struct { v2T_t rg, ba; };
	struct { float __3; v2T_t gb; };
	struct { v3T_t rgb; };
	struct { float __4; v3T_t yza; };
	struct { float u, v, w; };
	struct { v2T_t uv; };
	struct { float __5; v2T_t vw; };
	struct { v3T_t uvw; };

	float arr[4];
} v4T_t; qsassert(sizeof(v4T_t) == 4*sizeof(float), "v4 != 16bytes");

typedef union {
	struct { float x, y; };
	struct { float r, g; };
	struct { float u, v; };
	float arr[2];
	v2T_t T;
} v2_t; qsassert(sizeof(v2_t) == 2*sizeof(float), "v2 != 8bytes");

typedef union {
	struct { float x, y, z; };
	struct { v2_t xy; float __1; };
	struct { float __2; v2_t yz; };
	struct { float __3, g; };
	struct { v2_t rg; float b; };
	struct { float r; v2_t gb; };
	struct { float __4, v; };
	struct { v2_t uv; float w; };
	struct { float u; v2_t vw; };
	float arr[3];
	v3T_t T;
} v3_t; qsassert(sizeof(v3_t) == 3*sizeof(float), "v3 != 12bytes");

typedef union {
	struct { float x, y, z, W; };
	struct { v2_t xy, zW; };
	struct { float __1; v2_t yz; };
	struct { v3_t xyz; };
	struct { float __2; v3_t yzW; };
	struct { float r, g, b, a; };
	struct { v2_t rg, ba; };
	struct { float __3; v2_t gb; };
	struct { v3_t rgb; };
	struct { float __4; v3_t yza; };
	struct { float u, v, w; };
	struct { v2_t uv; };
	struct { float __5; v2_t vw; };
	struct { v3_t uvw; };
	float arr[4];
	v4T_t T;
} v4_t; qsassert(sizeof(v4_t) == 4*sizeof(float), "v4 != 16bytes");

C_LINK_START

static v2_t
v2_add(v2_t l, v2_t r) {
	v2_t res = {{ l.x+r.x, l.y+r.y }};
	return res;
}
static v3_t
v3_add(v3_t l, v3_t r) {
	v3_t res = {{ l.x+r.x, l.y+r.y, l.z+r.z }};
	return res;
}
static v4_t
v4_add(v4_t l, v4_t r) {
	v4_t res = {{ l.x+r.x, l.y+r.y, l.z+r.z, l.W+r.W }};
	return res;
}

static float
v2_len_squ(v2_t v) {
	float res = v.x*v.x + v.y*v.y;
	return res;
}
static float
v3_len_squ(v3_t v) {
	float res = v.x*v.x + v.y*v.y + v.z*v.z;
	return res;
}
static float
v4_len_squ(v4_t v) {
	float res = v.x*v.x + v.y*v.y + v.z*v.z + v.W*v.W;
	return res;
}

#include <math.h>
static float
v2_len(v2_t v) {
	float res = (float)sqrt((float)v.x*v.x + v.y*v.y);
	return res;
}
static float
v3_len(v3_t v) {
	float res = (float)sqrt((float)v.x*v.x + v.y*v.y + v.z*v.z);
	return res;
}
static float
v4_len(v4_t v) {
	float res = (float)sqrt((float)v.x*v.x + v.y*v.y + v.z*v.z + v.W*v.W);
	return res;
}

static v2_t
v2_norm_or_zero(v2_t v) {
	float len = v2_len(v);
	v.x*=len; v.y*=len;
	return v;
}
static v3_t
v3_norm_or_zero(v3_t v) {
	float len = v3_len(v);
	v.x*=len; v.y*=len; v.z*=len;
	return v;
}
static v4_t
v4_norm_or_zero(v4_t v) {
	float len = v4_len(v);
	v.x*=len; v.y*=len; v.z*=len; v.W*=len;
	return v;
}

static v3_t
v3_cross(v3_t l, v3_t r) {
	v3_t res = {{l.y*r.z - l.z*r.y,
	             l.z*r.x - l.x*r.z,
	             l.x*r.y - l.y*r.x}};
	return res;
}

C_LINK_END

#endif
