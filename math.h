#ifndef H_MATH
#define H_MATH

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592654f
#endif

#define len_v2(v) (sqrt((v)[0]*(v)[0] + (v)[1]*(v)[1]))

#define copy_m4_m4(r, a) memcpy(r, a, sizeof(float)*16)

void mul_m4_m4m4(float *r, const float *a, const float *b);
void mul_m4_m4(float *r, const float *a);

void inv_orthonormal_m4_m4(float *r, const float *a);

void create_identity_m4(float *r);
void create_translate_m4(float *r, float x, float y, float z);
void create_rotate_z_m4(float *r, float a);

void translate_m4(float *r, float x, float y, float z);
void rotate_z_m4(float *r, float a);

void print_m4(float *r);

#endif //H_MATH
