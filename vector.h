#ifndef VECTOR_H
#define VECTOR_H

#ifndef VECTOR_INTERNAL

typedef void* vector;

#endif /* VECTOR_INTERNAL */


/* *********************************************************************
 *              Constructors, destructor and accessors
 **********************************************************************/
vector vector_create(uint32 size);
vector vector_create_from_buffer(uint32 size, const real* buf);
vector vector_alias_buffer(uint32 size, real* buf);
vector* vector_allocate(uint32 size, uint32 num);
void vector_destroy(vector v);
uint32 vector_size(const vector v);
real* vector_buffer(const vector v);
real vector_get(const vector v, uint32 index);
void vector_set(vector v, uint32 index, real newval);
void vector_copy(vector dest, const vector source);
/**********************************************************************/

/***********************************************************************
 *        Primitive elementwise arithmetic operations                  *
 **********************************************************************/
void vector_add(vector u, uint32 u_stride,
        const vector v, uin32 v_stride, const vector w, uin32 w_stride);
void vector_mul(vector u, uint32 u_stride,
        const vector v, uin32 v_stride, const vector w, uin32 w_stride);
void vector_sub(vector u, uint32 u_stride,
        const vector v, uin32 v_stride, const vector w, uin32 w_stride);
void vector_div(vector u, uint32 u_stride,
        const vector v, uin32 v_stride, const vector w, uin32 w_stride);

/***********************************************************************
 *                      Basic operations                               *
 **********************************************************************/
void vector_scale(vector v, uint32 u_stride, real x);
void vector_add_scalar(vector u, uint32 u_stride, real x);
void vector_add_scaled(vector u, uint32 u_stide,
        const vector v, uint32 v_stride, real x);
/**********************************************************************/

#define VGET(vec,ind) vector_get(vec, ind)
#define VSET(vec, ind, val) vector_set(vec, ind, val)

#endif
