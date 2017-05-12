//
//  Matrix.h
//
//  Created by Kyle Cutler on 2017/04/20.
//

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <cstring>
#include <cmath>

//*****************//
//  HELPER MACROS  //
//*****************//

// some helpful macros for repeatedly applying a macro (such as ZIP or MAP)
#define _MATRIX_EVAL(x, ...) x(__VA_ARGS__)
#define _MATRIX_JOIN(n, ...) _MATRIX_JOIN_##n(__VA_ARGS__)
#define _MATRIX_JOIN_9(func, join, ...) _MATRIX_JOIN_8(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 8, __VA_ARGS__)
#define _MATRIX_JOIN_8(func, join, ...) _MATRIX_JOIN_7(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 7, __VA_ARGS__)
#define _MATRIX_JOIN_7(func, join, ...) _MATRIX_JOIN_6(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 6, __VA_ARGS__)
#define _MATRIX_JOIN_6(func, join, ...) _MATRIX_JOIN_5(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 5, __VA_ARGS__)
#define _MATRIX_JOIN_5(func, join, ...) _MATRIX_JOIN_4(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 4, __VA_ARGS__)
#define _MATRIX_JOIN_4(func, join, ...) _MATRIX_JOIN_3(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 3, __VA_ARGS__)
#define _MATRIX_JOIN_3(func, join, ...) _MATRIX_JOIN_2(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 2, __VA_ARGS__)
#define _MATRIX_JOIN_2(func, join, ...) _MATRIX_JOIN_1(func, join, __VA_ARGS__) join _MATRIX_EVAL(func, 1, __VA_ARGS__)
#define _MATRIX_JOIN_1(func, join, ...)                                              _MATRIX_EVAL(func, 0, __VA_ARGS__)

// the difference between JOIN and JOINC is that JOINC uses a comma as the join.
// this is necessary because passing a comma to a macro is ... difficult.
#define _MATRIX_JOINC(n, ...) _MATRIX_JOINC_##n(__VA_ARGS__)
#define _MATRIX_JOINC_9(func, ...) _MATRIX_JOINC_8(func, __VA_ARGS__) , _MATRIX_EVAL(func, 8, __VA_ARGS__)
#define _MATRIX_JOINC_8(func, ...) _MATRIX_JOINC_7(func, __VA_ARGS__) , _MATRIX_EVAL(func, 7, __VA_ARGS__)
#define _MATRIX_JOINC_7(func, ...) _MATRIX_JOINC_6(func, __VA_ARGS__) , _MATRIX_EVAL(func, 6, __VA_ARGS__)
#define _MATRIX_JOINC_6(func, ...) _MATRIX_JOINC_5(func, __VA_ARGS__) , _MATRIX_EVAL(func, 5, __VA_ARGS__)
#define _MATRIX_JOINC_5(func, ...) _MATRIX_JOINC_4(func, __VA_ARGS__) , _MATRIX_EVAL(func, 4, __VA_ARGS__)
#define _MATRIX_JOINC_4(func, ...) _MATRIX_JOINC_3(func, __VA_ARGS__) , _MATRIX_EVAL(func, 3, __VA_ARGS__)
#define _MATRIX_JOINC_3(func, ...) _MATRIX_JOINC_2(func, __VA_ARGS__) , _MATRIX_EVAL(func, 2, __VA_ARGS__)
#define _MATRIX_JOINC_2(func, ...) _MATRIX_JOINC_1(func, __VA_ARGS__) , _MATRIX_EVAL(func, 1, __VA_ARGS__)
#define _MATRIX_JOINC_1(func, ...)                                      _MATRIX_EVAL(func, 0, __VA_ARGS__)

// zip two indexable objects together using some operation.
#define _MATRIX_ZIP(i, l, r, op) (l[i] op r[i])

// map an indexable object using some left-handed operation.
#define _MATRIX_LMAP(i, s, op) (op s[i])

// map an indexable object using some right-handed operation.
#define _MATRIX_RMAP(i, s, op) (s[i] op)

//*********************//
//  DEFINITION MACROS  //
//*********************//

//***********//
//  VECTORS  //
//***********//

// defines an n-element vector, as well as several operations that can be
// performed on these vectors.
#define DEFINE_VEC(n, ...) \
    _MATRIX_DEFINE_VEC_STRUCT(n, __VA_ARGS__);                       \
    _MATRIX_DEFINE_VEC_OP_ADD(n)         /* vector addition */       \
    _MATRIX_DEFINE_VEC_OP_SUBTRACT(n)    /* vector subtraction */    \
    _MATRIX_DEFINE_VEC_OP_SCALE(n)       /* scalar multiplication */ \
    _MATRIX_DEFINE_VEC_OP_DOT_PRODUCT(n) /* vector dot product */    \
    _MATRIX_DEFINE_VEC_OP_LENGTH(n)      /* vector length */         \
    _MATRIX_DEFINE_VEC_OP_NORMALIZE(n)   /* vector normalization */  \
    _MATRIX_DEFINE_VEC_OP_EQUAL(n)       /* vector equality */

// defines an n-element vector
#define _MATRIX_DEFINE_VEC_STRUCT(n, ...) \
    typedef struct Vec##n { \
        float vals[n]; \
        /* index into this vector */ \
        inline const float& operator[](const int index) const { \
            return vals[index]; \
        } \
        /* write this vector to an array */ \
        inline void write(float *out) { \
            std::memcpy(out, vals, n * sizeof(float)); \
        } \
        __VA_ARGS__ \
    } Vec##n

///
// Element-wise addition between two vectors.
//
// @param l The first vector to use in the operation
// @param r The second vector to use in the operation
//
// @return  The result of adding the two vectors.
///
#define _MATRIX_DEFINE_VEC_OP_ADD(n) \
    inline Vec##n operator+(const Vec##n &l, const Vec##n &r) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, +) }; \
    }

///
// Element-wise subtraction between two vectors.
//
// @param l The first vector to use in the operation
// @param r The second vector to use in the operation
//
// @return  The result of subtracting the two vectors (l - r).
///
#define _MATRIX_DEFINE_VEC_OP_SUBTRACT(n) \
    inline Vec##n operator-(const Vec##n &l, const Vec##n &r) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, -) }; \
    }

///
// Dot product of two vectors.
//
// @param l The first vector
// @param r The second vector
//
// @return  The dot product of the two vectors
///
#define _MATRIX_DEFINE_VEC_OP_DOT_PRODUCT(n) \
    inline float operator*(const Vec##n &l, const Vec##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, +, l, r, *); \
    }

///
// Scalar multiplication of a vector.
//
// @param v The vector to scale
// @param s The scaling factor
//
// @return  The scaled vector
///
#define _MATRIX_DEFINE_VEC_OP_SCALE(n) \
    inline Vec##n operator*(const Vec##n &v, const float s) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_RMAP, v, * s) }; \
    } \
    inline Vec##n operator*(const float s, const Vec##n &v) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, v, s *) }; \
    }

///
// Vector length calculation
//
// @param v The vector
//
// @return  The length of the vector
///
#define _MATRIX_DEFINE_VEC_OP_LENGTH(n) \
    inline float operator+(const Vec##n &v) { \
        return sqrt(v * v); \
    }

///
// Vector normalization
//
// @param v The vector to normalize
//
// @return  The normalized vector
///
#define _MATRIX_DEFINE_VEC_OP_NORMALIZE(n) \
    inline Vec##n operator~(const Vec##n &v) { \
        float len = +v; \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_RMAP, v, / len) }; \
    }

///
// Vector equality
//
// @param l The first vector
// @param r The second vector
//
// @return  True iff the two vectors have all equal elements
///
#define _MATRIX_DEFINE_VEC_OP_EQUAL(n) \
    inline bool operator==(const Vec##n &l, const Vec##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, &&, l, r, ==); \
    } \
    inline bool operator!=(const Vec##n &l, const Vec##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, ||, l, r, !=); \
    }

//************//
//  MATRICES  //
//************//

// defines an nxn matrix, as well as several operations that can be
// performed on these matrices.
#define DEFINE_MAT(n, ...) \
    _MATRIX_DEFINE_MAT_STRUCT(n, __VA_ARGS__);                            \
    _MATRIX_DEFINE_MAT_OP_ADD(n)       /* matrix addition */              \
    _MATRIX_DEFINE_MAT_OP_SUBTRACT(n)  /* matrix subtraction */           \
    _MATRIX_DEFINE_MAT_OP_SCALE(n)     /* scalar multiplication */        \
    _MATRIX_DEFINE_MAT_OP_MULT_VEC(n)  /* vector-matrix multiplcation */  \
    _MATRIX_DEFINE_MAT_OP_MULT_MAT(n)  /* matrix-matrix multiplication */ \
    _MATRIX_DEFINE_MAT_OP_EQUAL(n)     /* matrix equality */

// macro for writing a column vector to part of an array
#define _MATRIX_MAT_WRITE(i, m, s, n) (m[i].write(&s[i * n]))

// A column-majorly ordered nxn Matrix
#define _MATRIX_DEFINE_MAT_STRUCT(n, ...) \
    typedef struct Mat##n { \
        Vec##n cols[n]; \
        /* index into this matrix. Returns a column vector. */ \
        inline const Vec##n& operator[](const int index) const { \
            return cols[index]; \
        } \
        /* Write this matrix to an array. \
         * We must write each column separately, since C++ does not guarantee \
         * contiguous memory for arrays of structs. \
         */ \
        inline void write(float *out) { \
            _MATRIX_JOIN(n, _MATRIX_MAT_WRITE, ;, cols, out, n); \
        } \
        __VA_ARGS__ \
    } Mat##n


///
// Element-wise addition between two matrices.
//
// @param l The first matrix to use in the operation
// @param r The second matrix to use in the operation
//
// @return  The result of adding the two matrices.
///
#define _MATRIX_DEFINE_MAT_OP_ADD(n) \
    inline Mat##n operator+(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, +) }; \
    }

///
// Element-wise subtraction between two matrices.
//
// @param l The first matrix to use in the operation
// @param r The second matrix to use in the operation
//
// @return  The result of subtracting the two matrices (l - r).
///
#define _MATRIX_DEFINE_MAT_OP_SUBTRACT(n) \
    inline Mat##n operator-(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, -) }; \
    }

///
// Scalar multiplication of a matrix.
//
// @param v The matrix to scale
// @param s The scaling factor
//
// @return  The scaled matrix
///
#define _MATRIX_DEFINE_MAT_OP_SCALE(n) \
    inline Mat##n operator*(const Mat##n &v, const float s) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_RMAP, v, * s) }; \
    } \
    inline Mat##n operator*(const float s, const Mat##n &v) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, v, s *) }; \
    }

///
// Right and left multiplication of a vector with a matrix
//
// @param m The matrix to multiply
// @param v The vector to multiply
//
// @return  The result of the multiplication (m * v) or (v * m)
///
#define _MATRIX_DEFINE_MAT_OP_MULT_VEC(n) \
    inline Vec##n operator*(const Mat##n &m, const Vec##n v) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, +, m, v, *); \
    } \
    inline Vec##n operator*(const Vec##n v, const Mat##n &m) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, m, v *) }; \
    }

///
// Multiplication of two matrices
//
// @param l The left matrix to multiply
// @param r The right matrix to multiply
//
// @return  The result of the matrix multiplication (l * r)
///
#define _MATRIX_DEFINE_MAT_OP_MULT_MAT(n) \
    inline Mat##n operator*(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, r, l *) }; \
    }

///
// Matrix equality
//
// @param l The first matrix
// @param r The second matrix
//
// @return  True iff the two matrices have all equal elements
///
#define _MATRIX_DEFINE_MAT_OP_EQUAL(n) \
    inline bool operator==(const Mat##n &l, const Mat##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, &&, l, r, ==); \
    } \
    inline bool operator!=(const Mat##n &l, const Mat##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, ||, l, r, !=); \
    }

#endif