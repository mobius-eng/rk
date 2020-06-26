#include <stdio.h>
#include <math.h>
#include "env.h"
#include "vector.h"
#include "rk.h"

void print_vector(vector *v)
{
    integer i;
    printf("[ ");
    for (i=0; i < vector_length(v); i++) {
        printf("%f ", vector_at(v, i));
    }
    printf("]\n");
}

void oscillator(real t, vector *x, vector *dx, void *data)
{
    real *k = (real*)data;
    vector_set(dx, 0, vector_at(x, 1));
    vector_set(dx, 1, -k[0] * vector_at(x, 0));
}

static const real xbuf[] = {1.0, 0.0};

int main()
{
    vector u, v, w;
    integer i;
    float pos, true_pos;
    vector x;
    real k = 1.0;
    rk rk;
    u = vector_create(4);
    v = vector_create(4);
    w = vector_create(4);
    
    for (i = 0; i < 4; i++) {
        VSET(u, i, i);
        VSET(v, i, 0.2);
        VSET(w, i, 3.5*i);
    }

    vector_mmla(&u, 0.3, &v, 1.0);
    printf("u = ");
    print_vector(&u);

    x = vector_create_from_buffer(2, xbuf);
    rk = rk_create(&bt_rkf45, 2);
    printf("Time\t\tPos\t\tTrue pos\n");
    for (i = 0; i < 10; i++) {
        rk_fixed_step(&rk, oscillator, i*0.1, &x, 0.1, &k);
        pos = VGET(x, 0); 
        true_pos = cos((i+1) * 0.1);
        printf("%f\t%f\t%f\n", 0.1*(i+1), pos, true_pos);
    }
    
    vector_free(&u);
    vector_free(&v);
    vector_free(&w);

    vector_free(&x);
    rk_free(&rk);
    return 0;
}

