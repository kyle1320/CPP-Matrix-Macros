//
//  Matrix.h
//
//  Created by Kyle Cutler on 2017/04/20.
//

#ifndef _MATRIX_H_
#define _MATRIX_H_

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
    typedef struct Vec##n { \
        float vals[n]; \
        /* index into this vector */ \
        inline const float& operator[](const int index) const { \
            return vals[index]; \
        } \
        /* write this vector to an array */ \
        inline void write(float *out) { \
            _MATRIX_JOIN(n, _MATRIX_ZIP, ;, out, vals, =); \
        } \
        __VA_ARGS__ \
    } Vec##n; \
    /* vector addition */ \
    inline Vec##n operator+(const Vec##n &l, const Vec##n &r) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, +) }; \
    } \
    /* vector subtraction */ \
    inline Vec##n operator-(const Vec##n &l, const Vec##n &r) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, -) }; \
    } \
    /* vector dot product */ \
    inline float operator*(const Vec##n &l, const Vec##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, +, l, r, *); \
    } \
    /* vector-scalar multiplication */ \
    inline Vec##n operator*(const Vec##n &v, const float s) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_RMAP, v, * s) }; \
    } \
    /* scalar-vector multiplication */ \
    inline Vec##n operator*(const float s, const Vec##n &v) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, v, s *) }; \
    } \
    /* vector length calculation */ \
    inline float operator+(const Vec##n &v) { \
        return sqrt(v * v); \
    } \
    /* vector normalization */ \
    inline Vec##n operator~(const Vec##n &v) { \
        float len = +v; \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_RMAP, v, / len) }; \
    } \
    /* vector equality */ \
    inline bool operator==(const Vec##n &l, const Vec##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, &&, l, r, ==); \
    } \
    /* vector inequality */ \
    inline bool operator!=(const Vec##n &l, const Vec##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, ||, l, r, !=); \
    }

//************//
//  MATRICES  //
//************//

// macro for writing a column vector to part of an array
#define _MATRIX_MAT_WRITE(i, m, s, n) (m[i].write(&s[i * n]))

// defines an nxn column-majorly ordered matrix, as well as several
// operations that can be performed on these matrices.
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
            _MATRIX_JOIN(n, _MATRIX_MAT_WRITE, ;, cols, out, n); \
        } \
        __VA_ARGS__ \
    } Mat##n; \
    /* matrix addition */ \
    inline Mat##n operator+(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, +) }; \
    } \
    /* matrix subtraction */ \
    inline Mat##n operator-(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_ZIP, l, r, -) }; \
    } \
    /* matrix-scalar multiplcation */ \
    inline Mat##n operator*(const Mat##n &v, const float s) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_RMAP, v, * s) }; \
    } \
    /* scalar-matrix multiplcation */ \
    inline Mat##n operator*(const float s, const Mat##n &v) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, v, s *) }; \
    } \
    /* matrix-vector multiplication */ \
    inline Vec##n operator*(const Mat##n &m, const Vec##n v) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, +, m, v, *); \
    } \
    /* vector-matrix multiplication */ \
    inline Vec##n operator*(const Vec##n v, const Mat##n &m) { \
        return (Vec##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, m, v *) }; \
    } \
    /* matrix-matrix multiplcation */ \
    inline Mat##n operator*(const Mat##n &l, const Mat##n &r) { \
        return (Mat##n) { _MATRIX_JOINC(n, _MATRIX_LMAP, r, l *) }; \
    } \
    /* matrix equality */ \
    inline bool operator==(const Mat##n &l, const Mat##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, &&, l, r, ==); \
    } \
    /* matrix inequality */ \
    inline bool operator!=(const Mat##n &l, const Mat##n &r) { \
        return _MATRIX_JOIN(n, _MATRIX_ZIP, ||, l, r, !=); \
    }

#endif