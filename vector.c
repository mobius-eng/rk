#include <stdlib.h>
#include <string.h>
#include "env.h"
#define VECTOR_INTERNAL

typedef struct vector_tag {
    uint32 size;
    real *buf;
} *vector;

#include "vector.h"


vector vector_create(uin32 size)
{
    vector v;
    v = (vector)malloc(sizeof(*v) + size * sizeof(real));
    if (v == NULL) {
        return v;
    }
    else {
        v->size = size;
        v->buffer = (real*)&(v[1]);
    }
    return v;
}


vector vector_create_from_buffer(uin32 size, const real* buf)
{
    vector v;
    v = vector_create(size);
    if (v == NULL) return v;
    memcpy(v->buf, buf, size * sizeof(real));
    return v;
}


vector vector_alias_buffer(uin32 size, real* buf)
{
    vector v;
    v = (vector)malloc(sizeof(*v));
    if (v == NULL) return v;
    v->size = size;
    v->buf = buf;
    return v;
}


vector vector_create_multiple(uin32 size, uin32 num)
{
    vector *vecs;
    real* buf;
    uin32 i;
    vecs = (vector*)malloc(sizeof(**vecs)*num + size * sizeof(real) * num);
    buf = (real*)&(vecs[num]);
    if (vecs == NULL) return NULL;
    for (i = 0; i < num; i++)
    {
        vecs[i]->size = size;
        vecs[i]->buf = &(buf[i*size]);
    }
    return vecs;
}


void vector_destroy(vector v)
{
    free(v);
}

void vector_destroy_multiple(vector* vecs)
{
    free(vecs);
}


uin32 vector_size(vector* v)
{
    return v->size;
}


real* vector_buffer(vector* v)
{
    return v->buf;
}


real vector_at(vector *v, uin32 index)
{
    return v->buf[index];
}


void vector_set(vector *v, uin32 index, real newval)
{
    v->buf[index] = newval;
}


void vector_muls(vector* v, real x)
{
    uin32 i;
    for (i = 0; i < v->size; i++) {
        v->buf[i] *= x;
    }
}


void vector_mulv(vector* u, vector* v)
{
    uin32 i;
    for (i = 0; i < u->size; i++) {
        u->buf[i] *= v->buf[i];
    }
}


void vector_adds(vector* u, real x)
{
    uin32 i;
    for (i = 0; i < u->size; i++) {
        u->buf[i] += x;
    }
}


void vector_addv(vector* u, vector* v)
{
    uin32 i;
    for (i = 0; i < u->size; i++) {
        u->buf[i] += v->buf[i];
    }
}


void vector_mla(vector* u, vector* v, real x)
{
    uin32 i;
    for (i = 0; i < u->size; i++) {
        u->buf[i] += v->buf[i] * x;
    }
}


void vector_mmla(vector* u, real x, vector* v, real y)
{
    uin32 i;
    for (i = 0; i < u->size; i++) {
        u->buf[i] = x * u->buf[i] + y * v->buf[i];
    }
}


void vector_copy(vector *dest, vector *source)
{
    memcpy(dest->buf, source->buf, dest->size * sizeof(real));
}


vectorex vectorex_alias(vector* v, uin32 stride)
{
    vectorex w;
    w.vec = *v;
    w.stride = stride;
    return w;
}


uin32 vectorex_size(vectorex* v)
{
    return v->vec.size / v->stride;
}


real vectorex_at(vectorex *v, uin32 index)
{
    return v->vec.buf[index * v->stride];
}


void vectorex_set(vectorex *v, uin32 index, real newval)
{
    v->vec.buf[index * v->stride] = newval;
}


void vectorex_muls(vectorex* v, real x)
{
    uin32 i;
    for (i = 0; i < v->vec.size; i += v->stride) {
        v->vec.buf[i] *= x;
    }
}


void vectorex_mulv(vectorex* u, vectorex* v)
{
    uin32 i, j;
    for (i = 0, j = 0; i < u->vec.size; i += u->stride, j += v->stride) {
        u->vec.buf[i] *= v->vec.buf[j];
    }
}


void vectorex_adds(vectorex* u, real x)
{
    uin32 i;
    for (i = 0; i < u->vec.size; i += u->stride) {
        u->vec.buf[i] += x;
    }
}


void vectorex_addv(vectorex* u, vectorex* v)
{
    uin32 i, j;
    for (i = 0, j = 0; i < u->vec.size; i += u->stride, j += v->stride) {
        u->vec.buf[i] += v->vec.buf[j];
    }
}


void vectorex_mla(vectorex* u, vectorex* v, real x)
{
    uin32 i, j;
    for (i = 0, j = 0; i < u->vec.size; i += u->stride, j += v->stride) {
        u->vec.buf[i] += v->vec.buf[j] * x;
    }
}


void vectorex_mmla(vectorex* u, real x, vectorex* v, real y)
{
    uin32 i, j;
    for (i = 0, j = 0; i < u->vec.size; i += u->stride, j += v->stride) {
        u->vec.buf[i] = u->vec.buf[i] * y + v->vec.buf[j] * x;
    }
}
