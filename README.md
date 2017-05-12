# C++ Macro-Based Matrix Library

This library defines several macros for defining matrix and vector objects.

This way, you can choose to only define the vectors and matrices you are using.

Vectors up to 9 elements, and matrices up to 9x9 are supported (matrices must be square).

## Usage
```c++
// Simply include Matrix.h in any C++ project
#include "Matrix.h"

// Define a 3-element vector, as well as several operations including
// addition (+), subtraction (-), scalar multiplication (*), dot product (*),
// length retrieval (unary +), normalization (~), and equality testing (==).
DEFINE_VEC(3);

// Initialize using standard struct syntax.
Vec3 v1 = {1, 2, 3};
Vec3 v2 = {4, 5, 6};

// Perform some operations on the vectors.
Vec3 sum = v1 + v2;  // {5, 7, 9}
float dot = v1 * v2; // 32.0f

// Elements can be accessed using the subscript operator.
float y = v2[1]; // 5

// Now we can define a 3x3 matrix in a similar way.
// Note that Vec3 must be previously defined in order to define Mat3.
DEFINE_MAT(3);

// Initialize using struct syntax. Note that the extra braces are optional.
// Also note that matrices are defined in column-major order.
Mat3 m1 = {{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
}};
Mat3 m2 = {
    1, 0, 1,
    0, 1, 0,
    1, 0, 1
};

// Columns can be accessed using the subscript operator.
Vec3 col = m1[1]; // {4, 5, 6}

// perform some operations.
Vec3 prod = m2 * v1; // {4, 2, 4}
Mat3 comp = m1 * m2; // {{{8, 10, 12}, {4, 5, 6}, {8, 10, 12}}}

// To extract the data from a matrix into a 1-dimensional array,
// use the write() method on the Matrix struct.
float vals[9];
m1.write(vals);
```

## Operations

The following operations are defined for vectors and matices:

### Vector Operations

| Operation             | Usage                    |
| --------------------- | ------------------------ |
| Addition              | `v1 + v2`                |
| Subtraction           | `v1 - v2`                |
| Scalar Multiplication | `v * s` or `s * v`       |
| Dot Product           | `v1 * v2`                |
| Vector Length         | `+v`                     |
| Normalization         | `~v`                     |
| Equality Testing      | `v1 == v2` or `v1 != v2` |

### Matrix Operations

| Operation             | Usage                    |
| --------------------- | ------------------------ |
| Addition              | `m1 + m2`                |
| Subtraction           | `m1 - m2`                |
| Scalar Multiplication | `m * s` or `s * m`       |
| Vector Multiplication | `m * v` or `v * m`       |
| Matrix Multiplcation  | `m1 * m2`                |
| Equality Testing      | `m1 == m2` or `m1 != m2` |