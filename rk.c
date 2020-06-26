#include <stdlib.h>
#include "env.h"
#include "vector.h"
#include "rk.h"

/* Butcher tableau for RKF45 method */
static real cab_rkf45[] = {
    /* c */
    0.25, 3.0/8, 12.0/13, 1.0, 0.5,
    /* a */
          1.0/4,
         3.0/32,       9.0/32,
    1932.0/2197, -7200.0/2197,  7296.0/2197,
      439.0/216,         -8.0,   3680.0/513, -845.0/4104,
        -8.0/27,            2, -3544.0/2565, 1859.0/4104, -11.0/40,
    /* b */
    16.0/135, 0.0, 6656.0/12825, 28561.0/56430, -9.0/50, 2.0/55,
    /* b* */
    25.0/216, 0.0,  1408.0/2565,   2197.0/4104,  -1.0/5,    0.0};


butcher_tableau bt_rkf45 = 
    {6, cab_rkf45, cab_rkf45+5, cab_rkf45+20};

butcher_tableau rk_bt_create(integer size)
{
    real* buf;
    butcher_tableau bt;
    /* allocate one block of memory for everything */
    buf = (real*)malloc((2*size+size*(size-1)/2-1)*sizeof(real));
    if (buf == NULL) {
        bt.size = 0;
        bt.c = NULL;
        bt.a = NULL;
        bt.b = NULL;
        return bt;
    }
    bt.size = size;
    bt.c = buf;
    bt.a = buf + size - 1;
    bt.b = buf + size - 1 + size*(size-1)/2;
    return bt;
}


void rk_bt_free(butcher_tableau* bt)
{
    free(bt->c);
    bt->size = 0;
}


void rk_bt_set_c(butcher_tableau* bt, real* c)
{
    integer i;
    for (i = 0; i < bt->size-1; i++) {
        bt->c[i] = c[i];
    }
}


void rk_bt_set_a(butcher_tableau* bt, real* a)
{
    integer i, size;
    size = bt->size * (bt->size - 1) / 2;
    for (i = 0; i < size; i++) {
        bt->a[i] = a[i];
    }
}


void rk_bt_set_b(butcher_tableau* bt, real* b)
{
    integer i;
    for (i = 0; i < bt->size; i ++) {
        bt->b[i] = b[i];
    }
}


real rk_bt_c(butcher_tableau* bt, integer row)
{
    return bt->c[row-2];
}


real rk_bt_a(butcher_tableau* bt, integer row, integer col)
{
    row -= 2;
    col -= 1;
    return bt->a[row*(row+1)/2+col];
}


real rk_bt_b(butcher_tableau* bt, integer col)
{
    return bt->b[col-1];
}

rk rk_create(butcher_tableau *bt, integer vsize)
{
    rk rk;
    vector *vecs;
    rk.steps = bt->size;
    rk.bt = bt;
    vecs = vector_allocate(vsize, rk.steps + 1);
    rk.k = vecs;
    rk.x = vecs+rk.steps;
    return rk;
}


void rk_free(rk* rk)
{
    free(rk->k);
}


void rk_fixed_step(rk* rk, ode_rhs f, real t, vector* x, real h, void* data)
{
    integer s, i;
    real tt;

    tt = t;
    vector_copy(rk->x, x);
    for (s = 1; s < rk->steps; s++) {
        f(tt, rk->x, &(rk->k[s-1]), data);
        /* Next step */
        tt += rk_bt_c(rk->bt, s+1) * h;
        vector_copy(rk->x, x);
        for (i = 1; i < s+1; i++) {
            vector_mla(rk->x, &(rk->k[i-1]), rk_bt_a(rk->bt, s+1, i) * h);
        }
    }
    /* Compute last k */
    f(tt, rk->x, &(rk->k[s-1]), data);
    for (s = 1; s <= rk->steps; s++) {
        vector_mla(x, &(rk->k[s-1]), rk_bt_b(rk->bt, s) * h);
    }
}



butcher_tableau_embedded rk_bte_create(integer size, integer order)
{
    /* A little optimization (memory locality) requires allocation
     * of the whole block at once. Hence, cannot defer the constructor
     * to the base type */
    butcher_tableau_embedded bte;
    real* buf;
    bte.order = order;
    /* allocate one block of memory for everything */
    buf = (real*)malloc((3*size+size*(size-1)/2-1)*sizeof(real));
    if (buf == NULL) {
        bte.bt.size = 0;
        bte.bt.c = NULL;
        bte.bt.a = NULL;
        bte.bt.b = NULL;
        bte.bs = NULL;
        return bte;
    }
    bte.bt.size = size;
    bte.bt.c = buf;
    bte.bt.a = buf + size - 1;
    bte.bt.b = buf + size - 1 + size*(size-1)/2;
    bte.bs = buf + size - 1 + size*(size-1)/2 + size;
    return bte;
}


void rk_bte_free(butcher_tableau_embedded* bte);

void rk_bte_set_c(butcher_tableau_embedded* bt, real* c);
void rk_bte_set_a(butcher_tableau_embedded* bt, real* a);
void rk_bte_set_b(butcher_tableau_embedded* bt, real* b);
void rk_bte_set_bs(butcher_tableau_embedded* bt, real* b);

real rk_bte_c(butcher_tableau_embedded* bt, integer row);
real rk_bte_a(butcher_tableau_embedded* bt, integer row, integer col);
real rk_bte_b(butcher_tableau_embedded* bt, integer col);
real rk_bte_bs(butcher_tableau_embedded* bt, integer col);
