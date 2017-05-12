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
#define EVAL(x, ...) x(__VA_ARGS__)
#define JOIN(n, ...) JOIN_##n(__VA_ARGS__)
#define JOIN_9(func, join, ...) JOIN_8(func, join, __VA_ARGS__) join EVAL(func, 8, __VA_ARGS__)
#define JOIN_8(func, join, ...) JOIN_7(func, join, __VA_ARGS__) join EVAL(func, 7, __VA_ARGS__)
#define JOIN_7(func, join, ...) JOIN_6(func, join, __VA_ARGS__) join EVAL(func, 6, __VA_ARGS__)
#define JOIN_6(func, join, ...) JOIN_5(func, join, __VA_ARGS__) join EVAL(func, 5, __VA_ARGS__)
#define JOIN_5(func, join, ...) JOIN_4(func, join, __VA_ARGS__) join EVAL(func, 4, __VA_ARGS__)
#define JOIN_4(func, join, ...) JOIN_3(func, join, __VA_ARGS__) join EVAL(func, 3, __VA_ARGS__)
#define JOIN_3(func, join, ...) JOIN_2(func, join, __VA_ARGS__) join EVAL(func, 2, __VA_ARGS__)
#define JOIN_2(func, join, ...) JOIN_1(func, join, __VA_ARGS__) join EVAL(func, 1, __VA_ARGS__)
#define JOIN_1(func, join, ...)                                      EVAL(func, 0, __VA_ARGS__)

// the difference between JOIN and JOINC is that JOINC uses a comma as the join.
// this is necessary because passing a comma to a macro is ... difficult.
#define JOINC(n, ...) JOINC_##n(__VA_ARGS__)
#define JOINC_9(func, ...) JOINC_8(func, __VA_ARGS__) , EVAL(func, 8, __VA_ARGS__)
#define JOINC_8(func, ...) JOINC_7(func, __VA_ARGS__) , EVAL(func, 7, __VA_ARGS__)
#define JOINC_7(func, ...) JOINC_6(func, __VA_ARGS__) , EVAL(func, 6, __VA_ARGS__)
#define JOINC_6(func, ...) JOINC_5(func, __VA_ARGS__) , EVAL(func, 5, __VA_ARGS__)
#define JOINC_5(func, ...) JOINC_4(func, __VA_ARGS__) , EVAL(func, 4, __VA_ARGS__)
#define JOINC_4(func, ...) JOINC_3(func, __VA_ARGS__) , EVAL(func, 3, __VA_ARGS__)
#define JOINC_3(func, ...) JOINC_2(func, __VA_ARGS__) , EVAL(func, 2, __VA_ARGS__)
#define JOINC_2(func, ...) JOINC_1(func, __VA_ARGS__) , EVAL(func, 1, __VA_ARGS__)
#define JOINC_1(func, ...)                              EVAL(func, 0, __VA_ARGS__)

// zip two indexable objects together using some operation.
#define ZIP(i, l, r, op) (l[i] op r[i])

// map an indexable object using some left-handed operation.
#define LMAP(i, s, op) (op s[i])

// map an indexable object using some right-handed operation.
#define RMAP(i, s, op) (s[i] op)

//*********************//
//  DEFINITION MACROS  //
//*********************//

// defines an n-element vector
#define DEFINE_VEC(n, ...) \
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
    } Vec##n; \
    DEFINE_OP_VV2V(n, operator+, +) /* vector addition */       \
    DEFINE_OP_VV2V(n, operator-, -) /* vector subtraction */    \
    DEFINE_OP_SCALE(n)              /* scalar multiplication */ \
    DEFINE_OP_DOT(n)                /* vector dot product */    \
    DEFINE_OP_NORM(n)               /* vector normalization */

///
// Element-wise operation between two vectors.
// This could be addition or subtraction.
//
// @param l The first vector to use in the operation
// @param r The second vector to use in the operation
//
// @return  The result of applying the operation
///
#define DEFINE_OP_VV2V(n, def, op) \
    inline Vec##n def (const Vec##n &l, const Vec##n &r) { \
        return (Vec##n) { JOINC(n, ZIP, l, r, op) }; \
    }

///
// Dot product of two vectors.
//
// @param l The first vector
// @param r The second vector
//
// @return  The dot product of the two vectors
///
#define DEFINE_OP_DOT(n) \
    inline float operator*(const Vec##n &l, const Vec##n &r) { \
        return JOIN(n, ZIP, +, l, r, *); \
    }

///
// Scalar multiplication of a vector.
//
// @param v The vector to scale
// @param s The scaling factor
//
// @return  The scaled vector
///
#define DEFINE_OP_SCALE(n) \
    inline Vec##n operator*(const Vec##n &v, const float s) { \
        return (Vec##n) { JOINC(n, RMAP, v, * s) }; \
    } \
    inline Vec##n operator*(const float s, const Vec##n &v) { \
        return (Vec##n) { JOINC(n, LMAP, v, s *) }; \
    }

///
// Vector normalization
//
// @param v The vector to normalize
//
// @return  The normalized vector
///
#define DEFINE_OP_NORM(n) \
    inline Vec##n operator~(const Vec##n &v) { \
        float len = sqrt(v * v); \
        return (Vec##n) { JOINC(n, RMAP, v, / len) }; \
    }

// macro for writing a column vector to part of an array
#define MAT_WRITE(i, m, s, n) (m[i].write(&s[i * n]))

// A column-majorly ordered nxn Matrix
#define DEFINE_MAT(n, ...) \
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
            JOIN(n, MAT_WRITE, ;, cols, out, n); \
        } \
        __VA_ARGS__ \
    } Mat##n; \
    DEFINE_OP_MV2V(n) \
    DEFINE_OP_MM2M(n)

///
// Right multiplication of a vector with a matrix
//
// @param m The matrix to multiply
// @param v The vector to multiply
//
// @return  The result of the right-multiplication (m * v)
///
#define DEFINE_OP_MV2V(n) \
    inline Vec##n operator*(const Mat##n &m, const Vec##n v) { \
        return JOIN(n, ZIP, +, m, v, *); \
    }

///
// Multiplication of two matrices
//
// @param l The left matrix to multiply
// @param r The right matrix to multiply
//
// @return  The result of the matrix multiplication (l * r)
///
#define DEFINE_OP_MM2M(n) \
    inline Mat##n operator*(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { JOINC(n, LMAP, r, l *) }; \
    }

#endif