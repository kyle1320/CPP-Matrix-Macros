#include "test.h"

#define _RETURN(i, ...) i
#define _ID(i) _ID_ ## i
#define _ID_0(...) 0
#define _ID_1(...) 1
#define _ID_2(...) 2
#define _ID_3(...) 3
#define _ID_4(...) 4
#define _ID_5(...) 5
#define _ID_6(...) 6
#define _ID_7(...) 7
#define _ID_8(...) 8
#define _ID_9(...) 9

#define _TEST_COUNT(n) _MATRIX_JOINC(n, _RETURN)
#define _TEST_REPEAT(n, c) _MATRIX_JOINC(n, _ID(c))

#define _EPSILON 1e-6

#define _TEST_DEF_VEC_CREATION(n) \
    bool testVectorCreation##n() { \
        Vec##n v = {_TEST_COUNT(n)}; \
        for (int i = 0; i < n; i++) { \
            if (v[i] != i) return false; \
        } \
        return true; \
    }

#define _TEST_DEF_VEC_ADDITION(n) \
    bool testVectorAddition##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = {_TEST_COUNT(n)}; \
        Vec##n v3 = v1 + v2; \
        Vec##n v4 = {_TEST_REPEAT(n, 4)}; \
        Vec##n v5 = v3 + v4; \
        for (int i = 0; i < n; i++) { \
            if (v3[i] != i * 2) return false; \
        } \
        for (int i = 0; i < n; i++) { \
            if (v5[i] != i * 2 + 4) return false; \
        } \
        return true; \
    }

#define _TEST_DEF_VEC_SUBTRACTION(n) \
    bool testVectorSubtraction##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = {_TEST_COUNT(n)}; \
        Vec##n v3 = v1 - v2; \
        Vec##n v4 = {_TEST_REPEAT(n, 4)}; \
        Vec##n v5 = v1 - v4; \
        for (int i = 0; i < n; i++) { \
            if (v3[i] != 0) return false; \
        } \
        for (int i = 0; i < n; i++) { \
            if (v5[i] !=  i - 4) return false; \
        } \
        return true; \
    }

#define _TEST_DEF_VEC_SCALING(n) \
    bool testVectorScaling##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = v1 * 2; \
        Vec##n v3 = 2 * v1; \
        for (int i = 0; i < n; i++) { \
            if (v2[i] != i * 2) return false; \
            if (v3[i] != i * 2) return false; \
        } \
        return true; \
    }

#define _TEST_DEF_VEC_DOT_PRODUCT(n) \
    bool testVectorDotProduct##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = {_TEST_COUNT(n)}; \
        Vec##n v3 = {_TEST_REPEAT(n, 2)}; \
        float dot = v1 * v2; \
        float target = 0; \
        for (int i = 0; i < n; i++) { \
            target += i * i; \
        } \
        if (dot != target) return false; \
        dot = v2 * v3; \
        target = n * (n - 1); \
        if (dot != target) return false; \
        return true; \
    }

#define _TEST_DEF_VEC_LENGTH(n) \
    bool testVectorLength##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = {_TEST_REPEAT(n, 1)}; \
        Vec##n v3 = {1}; \
        float len1 = +v1; \
        float len2 = +v2; \
        float len3 = +v3; \
        float target1 = 0; \
        for (int i = 0; i < n; i++) { \
            target1 += i * i; \
        } \
        if (fabs(len1 - sqrt(target1)) > _EPSILON) return false; \
        if (fabs(len2 - sqrt(n)) > _EPSILON) return false; \
        if (fabs(len3 - 1) > _EPSILON) return false; \
        return true; \
    }

#define _TEST_DEF_VEC_NORM(n) \
    bool testVectorNorm##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = {_TEST_REPEAT(n, 1)}; \
        Vec##n v3 = {4}; \
        Vec##n v4 = ~v1; \
        Vec##n v5 = ~v2; \
        Vec##n v6 = ~v3; \
        float sum1 = 0, sum2 = 0, sum3 = 0; \
        for (int i = 0; i < n; i++) { \
            sum1 += v4[i] * v4[i]; \
            sum2 += v5[i] * v5[i]; \
            sum3 += v6[i] * v6[i]; \
        } \
        if (fabs(sqrt(sum1) - 1) > _EPSILON) return false; \
        if (fabs(sqrt(sum2) - 1) > _EPSILON) return false; \
        if (fabs(sqrt(sum3) - 1) > _EPSILON) return false; \
        if (fabs(v6[0] - 1) > _EPSILON) return false; \
        return true; \
    }

#define _TEST_DEF_VEC_EQUAL(n) \
    bool testVectorEqual##n() { \
        Vec##n v1 = {_TEST_COUNT(n)}; \
        Vec##n v2 = {_TEST_COUNT(n)}; \
        Vec##n v3 = {_TEST_REPEAT(n, 3)}; \
        if (!(v1 == v2)) return false; \
        if (v1 != v2) return false; \
        if (v1 == v3) return false; \
        if (!(v1 != v3)) return false; \
        if (!((v1 - v2) == (v3 - v3))) return false; \
        if ((v1 - v2) != (v3 - v3)) return false; \
        return true; \
    }

#define _TEST_DEF_VEC_SIZE(n) \
    _TEST_DEF_VEC_CREATION(n) \
    _TEST_DEF_VEC_ADDITION(n) \
    _TEST_DEF_VEC_SUBTRACTION(n) \
    _TEST_DEF_VEC_SCALING(n) \
    _TEST_DEF_VEC_DOT_PRODUCT(n) \
    _TEST_DEF_VEC_LENGTH(n) \
    _TEST_DEF_VEC_NORM(n) \
    _TEST_DEF_VEC_EQUAL(n)

#define _TEST_CASE(n, name, func) \
    std::cout << "    " # name "... "; \
    if (!func##n()) { \
        std::cout << "FAILED" << std::endl; \
        return false; \
    } else { std::cout << "SUCCESS" << std::endl; }

#define _TEST_VEC_SIZE(n) \
    std::cout << "TESTING VEC" # n << std::endl; \
    _TEST_CASE(n, creation, testVectorCreation) \
    _TEST_CASE(n, addition, testVectorAddition) \
    _TEST_CASE(n, subtraction, testVectorSubtraction) \
    _TEST_CASE(n, scaling, testVectorScaling) \
    _TEST_CASE(n, dot product, testVectorDotProduct) \
    _TEST_CASE(n, length, testVectorLength) \
    _TEST_CASE(n, normalization, testVectorNorm) \
    _TEST_CASE(n, equality, testVectorEqual)

_TEST_DEF_VEC_SIZE(1);
_TEST_DEF_VEC_SIZE(2);
_TEST_DEF_VEC_SIZE(3);
_TEST_DEF_VEC_SIZE(4);
_TEST_DEF_VEC_SIZE(5);
_TEST_DEF_VEC_SIZE(6);
_TEST_DEF_VEC_SIZE(7);
_TEST_DEF_VEC_SIZE(8);
_TEST_DEF_VEC_SIZE(9);

bool testVectorOperations() {
    _TEST_VEC_SIZE(1);
    _TEST_VEC_SIZE(2);
    _TEST_VEC_SIZE(3);
    _TEST_VEC_SIZE(4);
    _TEST_VEC_SIZE(5);
    _TEST_VEC_SIZE(6);
    _TEST_VEC_SIZE(7);
    _TEST_VEC_SIZE(8);
    _TEST_VEC_SIZE(9);

    return true;
}