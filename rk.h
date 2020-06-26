#ifndef RK_H
#define RK_H


typedef struct butcher_tableau_tag {
    integer size;
    real* c;
    real* a;
    real* b;
} butcher_tableau;

butcher_tableau rk_bt_create(integer size);
void rk_bt_free(butcher_tableau* bt);

void rk_bt_set_c(butcher_tableau* bt, real* c);
void rk_bt_set_a(butcher_tableau* bt, real* a);
void rk_bt_set_b(butcher_tableau* bt, real* b);

real rk_bt_c(butcher_tableau* bt, integer row);
real rk_bt_a(butcher_tableau* bt, integer row, integer col);
real rk_bt_b(butcher_tableau* bt, integer col);

/* Pre-defined Butcher tableaus */
extern butcher_tableau bt_rkf45;

/* Type for ODE RHS function */
typedef void (*ode_rhs)(real t, vector* x, vector* dxdt, void* data); 

typedef struct rk_tag {
    integer steps;
    butcher_tableau *bt;
    vector *k;
    vector *x;
} rk;


rk rk_create(butcher_tableau *bt, integer vsize);
void rk_free(rk* rk);
void rk_fixed_step(rk* rk, ode_rhs f, real t, vector* x, real h, void* data);

/* Adaptive Runge-Kutta */
/* Embedded Butcher tableau */
typedef struct butcher_tableau_embedded_tag {
    butcher_tableau bt;
    /* Lower order b */
    real *bs;
    integer order;
} butcher_tableau_embedded;

butcher_tableau_embedded rk_bte_create(integer size, integer order);
void rk_bte_free(butcher_tableau_embedded* bte);

void rk_bte_set_c(butcher_tableau_embedded* bt, real* c);
void rk_bte_set_a(butcher_tableau_embedded* bt, real* a);
void rk_bte_set_b(butcher_tableau_embedded* bt, real* b);
void rk_bte_set_bs(butcher_tableau_embedded* bt, real* b);

real rk_bte_c(butcher_tableau_embedded* bt, integer row);
real rk_bte_a(butcher_tableau_embedded* bt, integer row, integer col);
real rk_bte_b(butcher_tableau_embedded* bt, integer col);
real rk_bte_bs(butcher_tableau_embedded* bt, integer col);

/* Error calculation: weighted root mean square */
real rk_vnorm(vector* v, vector* weights);

typedef struct rk_adaptive_tag {
    rk base;
    vector* error_weights;
    vector* rtol;
    real atol;
} rk_adaptive;

rk_adaptive rk_adaptive_create(
        butcher_tableau_embedded* bte,
        integer vsize,
        real atol,
        real* rtol,
        bool is_scalar_rtol);
void rk_adaptive_free(rk_adaptive* rk);

void rk_adaptive_step(rk_adaptive* rk, ode_rhs f, real* t, vector* x,
        real* h, void *data);

#endif
