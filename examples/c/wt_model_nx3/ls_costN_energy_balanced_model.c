/* This file was automatically generated by CasADi.
   The CasADi copyright holders make no ownership claim of its contents. */
#ifdef __cplusplus
extern "C" {
#endif

/* How to prefix internal symbols */
#ifdef CODEGEN_PREFIX
  #define NAMESPACE_CONCAT(NS, ID) _NAMESPACE_CONCAT(NS, ID)
  #define _NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) ls_costN_energy_balanced_model_ ## ID
#endif

#include <math.h>

#ifndef casadi_real
#define casadi_real double
#endif

#define to_double(x) (double) x
#define to_int(x) (int) x
#define CASADI_CAST(x,y) (x) y

/* Pre-c99 compatibility */
#if __STDC_VERSION__ < 199901L
  #define fmin CASADI_PREFIX(fmin)
  casadi_real fmin(casadi_real x, casadi_real y) { return x<y ? x : y;}
  #define fmax CASADI_PREFIX(fmax)
  casadi_real fmax(casadi_real x, casadi_real y) { return x>y ? x : y;}
#endif

/* CasADi extensions */
#define sq CASADI_PREFIX(sq)
casadi_real sq(casadi_real x) { return x*x;}
#define sign CASADI_PREFIX(sign)
casadi_real CASADI_PREFIX(sign)(casadi_real x) { return x<0 ? -1 : x>0 ? 1 : x;}
#define twice CASADI_PREFIX(twice)
casadi_real twice(casadi_real x) { return x+x;}
#define if_else CASADI_PREFIX(if_else)
casadi_real if_else(casadi_real c, casadi_real x, casadi_real y) { return c!=0 ? x : y;}

/* Add prefix to internal symbols */
#define casadi_copy CASADI_PREFIX(copy)
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_fill CASADI_PREFIX(fill)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_trans CASADI_PREFIX(trans)

/* Printing routine */
#define PRINTF printf

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

static const int casadi_s0[9] = {3, 3, 0, 1, 2, 3, 0, 1, 2};
static const int casadi_s1[7] = {3, 1, 0, 3, 0, 1, 2};

void casadi_copy(const casadi_real* x, int n, casadi_real* y) {
  int i;
  if (y) {
    if (x) {
      for (i=0; i<n; ++i) *y++ = *x++;
    } else {
      for (i=0; i<n; ++i) *y++ = 0.;
    }
  }
}

void casadi_fill(casadi_real* x, int n, casadi_real alpha) {
  int i;
  if (x) {
    for (i=0; i<n; ++i) *x++ = alpha;
  }
}

void casadi_trans(const casadi_real* x, const int* sp_x, casadi_real* y, const int* sp_y, int* tmp) {
  int ncol_x = sp_x[1];
  int nnz_x = sp_x[2 + ncol_x];
  const int* row_x = sp_x + 2 + ncol_x+1;
  int ncol_y = sp_y[1];
  const int* colind_y = sp_y+2;
  int k;
  for (k=0; k<ncol_y; ++k) tmp[k] = colind_y[k];
  for (k=0; k<nnz_x; ++k) {
    y[tmp[row_x[k]]++] = x[k];
  }
}

/* ls_costN_energy_balanced_model:(i0[3])->(o0[3],o1[3x3,3nz]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, int* iw, casadi_real* w, void* mem) {
  casadi_real *rr, *ss;
  casadi_real *w0=w+0, *w1=w+3;
  /* #0: @0 = input[0][0] */
  casadi_copy(arg[0], 3, w0);
  /* #1: output[0][0] = @0 */
  casadi_copy(w0, 3, res[0]);
  /* #2: @0 = zeros(3x3,3nz) */
  casadi_fill(w0, 3, 0.);
  /* #3: @1 = ones(3x1) */
  casadi_fill(w1, 3, 1.);
  /* #4: (@0[:3] = @1) */
  for (rr=w0+0, ss=w1; rr!=w0+3; rr+=1) *rr = *ss++;
  /* #5: @1 = @0' */
  casadi_trans(w0,casadi_s0, w1, casadi_s0, iw);
  /* #6: output[1][0] = @1 */
  casadi_copy(w1, 3, res[1]);
  return 0;
}

CASADI_SYMBOL_EXPORT int ls_costN_energy_balanced_model(const casadi_real** arg, casadi_real** res, int* iw, casadi_real* w, void* mem){
  return casadi_f0(arg, res, iw, w, mem);
}

CASADI_SYMBOL_EXPORT void ls_costN_energy_balanced_model_incref(void) {
}

CASADI_SYMBOL_EXPORT void ls_costN_energy_balanced_model_decref(void) {
}

CASADI_SYMBOL_EXPORT int ls_costN_energy_balanced_model_n_in(void) { return 1;}

CASADI_SYMBOL_EXPORT int ls_costN_energy_balanced_model_n_out(void) { return 2;}

CASADI_SYMBOL_EXPORT const char* ls_costN_energy_balanced_model_name_in(int i){
  switch (i) {
    case 0: return "i0";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* ls_costN_energy_balanced_model_name_out(int i){
  switch (i) {
    case 0: return "o0";
    case 1: return "o1";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const int* ls_costN_energy_balanced_model_sparsity_in(int i) {
  switch (i) {
    case 0: return casadi_s1;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const int* ls_costN_energy_balanced_model_sparsity_out(int i) {
  switch (i) {
    case 0: return casadi_s1;
    case 1: return casadi_s0;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT int ls_costN_energy_balanced_model_work(int *sz_arg, int* sz_res, int *sz_iw, int *sz_w) {
  if (sz_arg) *sz_arg = 3;
  if (sz_res) *sz_res = 3;
  if (sz_iw) *sz_iw = 4;
  if (sz_w) *sz_w = 6;
  return 0;
}


#ifdef __cplusplus
} /* extern "C" */
#endif
