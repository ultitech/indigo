#include "math.h"

#include <stdio.h>

void mul_m4_m4m4(float *r, const float *a, const float *b)
{
	r[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] + a[12]*b[3];
	r[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] + a[13]*b[3];
	r[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
	r[3] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];
	r[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6] + a[12]*b[7];
	r[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6] + a[13]*b[7];
	r[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
	r[7] = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];
	r[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
	r[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
	r[10] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
	r[11] = a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];
	r[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12]*b[15];
	r[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13]*b[15];
	r[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
	r[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];
}

void mul_m4_m4(float *r, const float *a)
{
	float t[16];
	mul_m4_m4m4(t, r, a);
	copy_m4_m4(r, t);
}

void inv_orthonormal_m4_m4(float *r, const float *a)
{
	r[0] = a[0];
	r[1] = a[4];
	r[2] = a[8];
	r[3] = 0.0f;
	r[4] = a[1];
	r[5] = a[5];
	r[6] = a[9];
	r[7] = 0.0f;
	r[8] = a[2];
	r[9] = a[6];
	r[10] = a[10];
	r[11] = 0.0f;
	r[12] = -(r[0]*a[12] + r[4]*a[13] + r[8]*a[14]);
	r[13] = -(r[1]*a[12] + r[5]*a[13] + r[9]*a[14]);
	r[14] = -(r[2]*a[12] + r[6]*a[13] + r[10]*a[14]);
	r[15] = 1.0f;
}

void create_identity_m4(float *r)
{
	r[1] = r[2] = r[3] = r[4] = r[6] = r[7] = r[8] = r[9] = r[11] = r[12] =
		r[13] = r[14] = 0.0f;
	r[0] = r[5] = r[10] = r[15] = 1.0f;
}

void create_translate_m4(float *r, float x, float y, float z)
{
	r[1] = r[2] = r[3] = r[4] = r[6] = r[7] = r[8] = r[9] = r[11] = 0.0f;
	r[0] = r[5] = r[10] = r[15] = 1.0f;
	r[12] = x; r[13] = y; r[14] = z;
}

void create_rotate_z_m4(float *r, float a)
{
	r[2] = r[3] = r[6] = r[7] = r[8] = r[9] = r[11] = r[12] = r[13] =
		r[14] = 0.0f;
	
	r[0] = cos(a);
	r[1] = sin(a);
	r[4] = -sin(a);
	r[5] = cos(a);
	
	r[10] = r[15] = 1.0f;
}

void translate_m4(float *r, float x, float y, float z)
{
	float t[16];
	create_translate_m4(t, x, y, z);
	mul_m4_m4(r, t);
}

void rotate_z_m4(float *r, float a)
{
	float t[16];
	create_rotate_z_m4(t, a);
	mul_m4_m4(r, t);
}

void print_m4(float *r)
{
	printf("%f %f %f %f\n", r[0], r[4], r[8], r[12]);
	printf("%f %f %f %f\n", r[1], r[5], r[9], r[13]);
	printf("%f %f %f %f\n", r[2], r[6], r[10], r[14]);
	printf("%f %f %f %f\n", r[3], r[7], r[11], r[15]);
}
