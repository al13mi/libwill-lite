#pragma once

#ifndef _DOUBLE_PRECISION
#define _SINGLE_PRECISION
typedef float real;
#else
typedef double real;
#endif

/*
    for convenience, all functions use similar parameters CUDA_FUNCTION{X}{Y}{H}, for examples:

    CUDA_FUNCTION22H(real* p1, real* p2, unsigned int size, real a1, real a2);
    CUDA_FUNCTION32(real* p1, real* p2, real* p3, unsigned int size, real a1, real a2)

    where X refers number of pointers and Y refers number of real parameters

    "H" means declaration, and here the semicolon should be followed at the end of the line

    add new macros if needed.
*/

#ifdef _NO_CUDA
#define CUDA_FUNCTION22H(name)
#define CUDA_FUNCTION32H(name)
#define CUDA_FUNCTION42H(name)

#define cuda_reciprocal (0)
#define cuda_addnumber (0)
#define cuda_pow (0)
#define cuda_sparse (0)
#define cuda_sign (0)
#define cuda_add (0)
#define cuda_mul (0)
#define cuda_div (0)
#define cuda_sectionlimit (0)
#define cuda_ada_update (0)
#define cuda_adaDelta_update (0)
#else
#define CUDA_FUNCTION22H(name)\
    int name(real* p1, real* p2, unsigned int size, real a1, real a2)
#define CUDA_FUNCTION32H(name)\
    int name(real* p1, real* p2, real* p3, unsigned int size, real a1, real a2)
#define CUDA_FUNCTION42H(name)\
    int name(real* p1, real* p2, real* p3, real* p4, unsigned int size, real a1, real a2)

CUDA_FUNCTION22H(cuda_reciprocal);
CUDA_FUNCTION22H(cuda_addnumber);
CUDA_FUNCTION22H(cuda_pow);
CUDA_FUNCTION22H(cuda_sparse);
CUDA_FUNCTION22H(cuda_sign);

CUDA_FUNCTION32H(cuda_add);
CUDA_FUNCTION32H(cuda_mul);
CUDA_FUNCTION32H(cuda_div);
CUDA_FUNCTION32H(cuda_sectionlimit);
CUDA_FUNCTION32H(cuda_ada_update);
CUDA_FUNCTION42H(cuda_adaDelta_update);
#endif



#undef CUDA_FUNCTION22H
#undef CUDA_FUNCTION32H
#undef CUDA_FUNCTION42H
