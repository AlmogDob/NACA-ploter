/* This one-file library is heavily inspired by Tsoding's nn.h implementation of matrix
creation and operation. you can find the source code in:
https://github.com/tsoding/nn.h .
featured in this video of his:
https://youtu.be/L1TbWe8bVOc?list=PLpM-Dvs8t0VZPZKggcql-MmjaBdZKeDMw .*/

/* NOTES:
 * There is a hole set of function for deling with the minors of a matrix because I tried to calculate the determinant of a matrix with them but it terns out to be TO SLOW. Insted I use Gauss elimination.
 * There are some stability problems in the inversion function. When the values of the matrix becomes too small, the inversion fails. Currently the only fix I can think of is to use pre-conditioners. However, it means to creat a function to solve the hole problem 'Ax=B', which wasn't my intention in the first place. I might do it but I am not sure. */

#ifndef MATRIX2D_H_
#define MATRIX2D_H_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#ifndef MATRIX2D_MALLOC
#define MATRIX2D_MALLOC malloc
#endif //MATRIX2D_MALLOC

#ifndef MATRIX2D_ASSERT
#include <assert.h>
#define MATRIX2D_ASSERT assert
#endif //MATRIX2D_ASSERT

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* how many element you need to traves to get to the element underneath */
    double *elements;
} Mat2D;

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* how many element you need to traves to get to the element underneath */
    uint32_t *elements;
} Mat2D_uint32;

typedef struct {
    size_t rows;
    size_t cols;
    size_t stride_r; /* how many element you need to traves to get to the element underneath */
    size_t *rows_list;
    size_t *cols_list;
    Mat2D ref_mat;
} Mat2D_Minor;

#if 0
#define MAT2D_AT(m, i, j) (m).elements[mat2D_offset2d((m), (i), (j))]
#define MAT2D_AT_UINT32(m, i, j) (m).elements[mat2D_offset2d_uint32((m), (i), (j))]
#else /* use this macro for batter performance but no assertion */
#define MAT2D_AT(m, i, j) (m).elements[i * m.stride_r + j]
#define MAT2D_AT_UINT32(m, i, j) (m).elements[i * m.stride_r + j]
#endif

#ifndef PI
#define PI M_PI
#endif

#define MAT2D_MINOR_AT(mm, i, j) MAT2D_AT(mm.ref_mat, mm.rows_list[i], mm.cols_list[j])
#define MAT2D_PRINT(m) mat2D_print(m, #m, 0)
#define MAT2D_UINT32_PRINT(m) mat2D_uint32_print(m, #m, 0)
#define MAT2D_PRINT_AS_COL(m) mat2D_print_as_col(m, #m, 0)
#define MAT2D_MINOR_PRINT(mm) mat2D_minor_print(mm, #mm, 0)

double rand_double(void);

Mat2D mat2D_alloc(size_t rows, size_t cols);
Mat2D_uint32 mat2D_alloc_uint32(size_t rows, size_t cols);
void mat2D_free(Mat2D m);
void mat2D_free_uint32(Mat2D_uint32 m);
size_t mat2D_offset2d(Mat2D m, size_t i, size_t j);
size_t mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j);

void mat2D_fill(Mat2D m, double x);
void mat2D_fill_sequence(Mat2D m, double start, double step);
void mat2D_rand(Mat2D m, double low, double high);

void mat2D_dot(Mat2D dst, Mat2D a, Mat2D b);
void mat2D_cross(Mat2D dst, Mat2D a, Mat2D b);

void mat2D_add(Mat2D dst, Mat2D a);
void mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor);

void mat2D_sub(Mat2D dst, Mat2D a);
void mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor);

void mat2D_mult(Mat2D m, double factor);
void mat2D_mult_row(Mat2D m, size_t r, double factor);

void mat2D_print(Mat2D m, const char *name, size_t padding);
void mat2D_uint32_print(Mat2D_uint32 m, const char *name, size_t padding);
void mat2D_print_as_col(Mat2D m, const char *name, size_t padding);

void mat2D_set_identity(Mat2D m);
double mat2D_make_identity(Mat2D m);
void mat2D_set_rot_mat_x(Mat2D m, float angle_deg);
void mat2D_set_rot_mat_y(Mat2D m, float angle_deg);
void mat2D_set_rot_mat_z(Mat2D m, float angle_deg);

void mat2D_copy(Mat2D des, Mat2D src);
void mat2D_copy_mat_to_mat_at_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je);
void mat2D_uint32_flip_y(Mat2D_uint32 m);

void mat2D_get_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);
void mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col);

void mat2D_swap_rows(Mat2D m, size_t r1, size_t r2);
void mat2D_uint32_swap_rows(Mat2D_uint32 m, size_t r1, size_t r2);
void mat2D_get_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);
void mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row);

double mat2D_calc_norma(Mat2D m);

bool mat2D_mat_is_all_digit(Mat2D m, double digit);
bool mat2D_row_is_all_digit(Mat2D m, double digit, size_t r);
bool mat2D_col_is_all_digit(Mat2D m, double digit, size_t c);

double mat2D_det_2x2_mat(Mat2D m);
double mat2D_triangulate(Mat2D m);
double mat2D_det(Mat2D m);
void mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u);
void mat2D_transpose(Mat2D des, Mat2D src);
void mat2D_invert(Mat2D des, Mat2D src);
void mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B);

Mat2D_Minor mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j);
Mat2D_Minor mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j);
void mat2D_minor_free(Mat2D_Minor mm);
void mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding);
double mat2D_det_2x2_mat_minor(Mat2D_Minor mm);
double mat2D_minor_det(Mat2D_Minor mm);

#endif // MATRIX2D_H_

#ifdef MATRIX2D_IMPLEMENTATION
#undef MATRIX2D_IMPLEMENTATION

double rand_double(void)
{
    return (double) rand() / (double) RAND_MAX;
}

Mat2D mat2D_alloc(size_t rows, size_t cols)
{
    Mat2D m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (double*)MATRIX2D_MALLOC(sizeof(double)*rows*cols);
    MATRIX2D_ASSERT(m.elements != NULL);
    
    return m;
}

Mat2D_uint32 mat2D_alloc_uint32(size_t rows, size_t cols)
{
    Mat2D_uint32 m;
    m.rows = rows;
    m.cols = cols;
    m.stride_r = cols;
    m.elements = (uint32_t*)MATRIX2D_MALLOC(sizeof(uint32_t)*rows*cols);
    MATRIX2D_ASSERT(m.elements != NULL);
    
    return m;
}

void mat2D_free(Mat2D m)
{
    free(m.elements);
}

void mat2D_free_uint32(Mat2D_uint32 m)
{
    free(m.elements);
}

size_t mat2D_offset2d(Mat2D m, size_t i, size_t j)
{
    MATRIX2D_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

size_t mat2D_offset2d_uint32(Mat2D_uint32 m, size_t i, size_t j)
{
    MATRIX2D_ASSERT(i < m.rows && j < m.cols);
    return i * m.stride_r + j;
}

void mat2D_fill(Mat2D m, double x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) = x;
        }
    }
}

void mat2D_fill_sequence(Mat2D m, double start, double step) {
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            MAT2D_AT(m, i, j) = start + step * mat2D_offset2d(m, i, j);
        }
    }
}

void mat2D_rand(Mat2D m, double low, double high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) = rand_double()*(high - low) + low;
        }
    }
}

void mat2D_dot(Mat2D dst, Mat2D a, Mat2D b)
{
    MATRIX2D_ASSERT(a.cols == b.rows);
    MATRIX2D_ASSERT(a.rows == dst.rows);
    MATRIX2D_ASSERT(b.cols == dst.cols);

    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            MAT2D_AT(dst, i, j) = 0;
            for (size_t k = 0; k < a.cols; k++) {
                MAT2D_AT(dst, i, j) += MAT2D_AT(a, i, k)*MAT2D_AT(b, k, j);
            }
        }
    }

}

void mat2D_cross(Mat2D dst, Mat2D a, Mat2D b)
{
    MATRIX2D_ASSERT(3 == dst.rows && 1 == dst.cols);
    MATRIX2D_ASSERT(3 == a.rows && 1 == a.cols);
    MATRIX2D_ASSERT(3 == b.rows && 1 == b.cols);

    MAT2D_AT(dst, 0, 0) = MAT2D_AT(a, 1, 0) * MAT2D_AT(b, 2, 0) - MAT2D_AT(a, 2, 0) * MAT2D_AT(b, 1, 0);
    MAT2D_AT(dst, 1, 0) = MAT2D_AT(a, 2, 0) * MAT2D_AT(b, 0, 0) - MAT2D_AT(a, 0, 0) * MAT2D_AT(b, 2, 0);
    MAT2D_AT(dst, 2, 0) = MAT2D_AT(a, 0, 0) * MAT2D_AT(b, 1, 0) - MAT2D_AT(a, 1, 0) * MAT2D_AT(b, 0, 0);
}

void mat2D_add(Mat2D dst, Mat2D a)
{
    MATRIX2D_ASSERT(dst.rows == a.rows);
    MATRIX2D_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT2D_AT(dst, i, j) += MAT2D_AT(a, i, j);
        }
    }
}

void mat2D_add_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        MAT2D_AT(m, des_r, j) += factor * MAT2D_AT(m, src_r, j);
    }
}

void mat2D_sub(Mat2D dst, Mat2D a)
{
    MATRIX2D_ASSERT(dst.rows == a.rows);
    MATRIX2D_ASSERT(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MAT2D_AT(dst, i, j) -= MAT2D_AT(a, i, j);
        }
    }
}

void mat2D_sub_row_time_factor_to_row(Mat2D m, size_t des_r, size_t src_r, double factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        MAT2D_AT(m, des_r, j) -= factor * MAT2D_AT(m, src_r, j);
    }
}

void mat2D_mult(Mat2D m, double factor)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) *= factor;
        }
    }
}

void mat2D_mult_row(Mat2D m, size_t r, double factor)
{
    for (size_t j = 0; j < m.cols; ++j) {
        MAT2D_AT(m, r, j) *= factor;
    }
}

void mat2D_print(Mat2D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%9.6f ", MAT2D_AT(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

void mat2D_uint32_print(Mat2D_uint32 m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            if (MAT2D_AT_UINT32(m, i, j)) {
                printf("%u ", MAT2D_AT_UINT32(m, i, j));
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

void mat2D_print_as_col(Mat2D m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows*m.cols; ++i) {
            printf("%*s    ", (int) padding, "");
            printf("%f\n", m.elements[i]);
    }
    printf("%*s]\n", (int) padding, "");
}

void mat2D_set_identity(Mat2D m)
{
    MATRIX2D_ASSERT(m.cols == m.rows);
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT2D_AT(m, i, j) = i == j ? 1 : 0;
            // if (i == j) {
            //     MAT2D_AT(m, i, j) = 1;
            // }
            // else {
            //     MAT2D_AT(m, i, j) = 0;
            // }
        }
    }
}

double mat2D_make_identity(Mat2D m)
{
    /* make identity matrix using Gauss elimination */
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    double factor_to_return = 1;

    for (size_t i = 0; i < (size_t)fmin(m.rows-1, m.cols); i++) {
        /* check if it is the biggest first number (absolute value) */
        size_t biggest_r = i;
        for (size_t index = i; index < m.rows; index++) {
            if (fabs(MAT2D_AT(m, index, index)) > fabs(MAT2D_AT(m, biggest_r, 0))) {
                biggest_r = index;
            }
        }
        if (i != biggest_r) {
            mat2D_swap_rows(m, i, biggest_r);
            factor_to_return *= -1;
        }
        for (size_t j = i+1; j < m.cols; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, MAT2D_AT(m, j, i) * factor);
            mat2D_mult_row(m, i, factor);
            factor_to_return *= factor;
        }
    }
    double factor = 1 / MAT2D_AT(m, m.rows-1, m.cols-1);
    mat2D_mult_row(m, m.rows-1, factor);
    factor_to_return *= factor;
    for (size_t c = m.cols-1; c > 0; c--) {
        for (int r = c-1; r >= 0; r--) {
            double factor = 1 / MAT2D_AT(m, c, c);
            mat2D_sub_row_time_factor_to_row(m, r, c, MAT2D_AT(m, r, c) * factor);
        }
    }


    return factor_to_return;
}

void mat2D_set_rot_mat_x(Mat2D m, float angle_deg)
{
    MATRIX2D_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * PI / 180;
    mat2D_set_identity(m);
    MAT2D_AT(m, 1, 1) =  cos(angle_rad);
    MAT2D_AT(m, 1, 2) =  sin(angle_rad);
    MAT2D_AT(m, 2, 1) = -sin(angle_rad);
    MAT2D_AT(m, 2, 2) =  cos(angle_rad);
}

void mat2D_set_rot_mat_y(Mat2D m, float angle_deg)
{
    MATRIX2D_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * PI / 180;
    mat2D_set_identity(m);
    MAT2D_AT(m, 0, 0) =  cos(angle_rad);
    MAT2D_AT(m, 0, 2) = -sin(angle_rad);
    MAT2D_AT(m, 2, 0) =  sin(angle_rad);
    MAT2D_AT(m, 2, 2) =  cos(angle_rad);
}

void mat2D_set_rot_mat_z(Mat2D m, float angle_deg)
{
    MATRIX2D_ASSERT(3 == m.cols && 3 == m.rows);

    float angle_rad = angle_deg * PI / 180;
    mat2D_set_identity(m);
    MAT2D_AT(m, 0, 0) =  cos(angle_rad);
    MAT2D_AT(m, 0, 1) =  sin(angle_rad);
    MAT2D_AT(m, 1, 0) = -sin(angle_rad);
    MAT2D_AT(m, 1, 1) =  cos(angle_rad);
}

void mat2D_copy(Mat2D des, Mat2D src)
{
    MATRIX2D_ASSERT(des.cols == src.cols);
    MATRIX2D_ASSERT(des.rows == src.rows);

    for (size_t i = 0; i < des.rows; ++i) {
        for (size_t j = 0; j < des.cols; ++j) {
            MAT2D_AT(des, i, j) = MAT2D_AT(src, i, j);
        }
    }
}

void mat2D_copy_mat_to_mat_at_window(Mat2D des, Mat2D src, size_t is, size_t js, size_t ie, size_t je)
{
    MATRIX2D_ASSERT(je > js && ie > is);
    MATRIX2D_ASSERT(je-js+1 == des.cols);
    MATRIX2D_ASSERT(ie-is+1 == des.rows);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            MAT2D_AT(des, index, jndex) = MAT2D_AT(src, is+index, js+jndex);
        }
    }
}

void mat2D_uint32_flip_y(Mat2D_uint32 m)
{
    for (size_t i = 0; i < m.rows/2; i++) {
        mat2D_uint32_swap_rows(m, i, m.rows-i-1);
    }
}

void mat2D_get_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MATRIX2D_ASSERT(src_col < src.cols);
    MATRIX2D_ASSERT(des.rows == src.rows);
    MATRIX2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) = MAT2D_AT(src, i, src_col);
    }
}

void mat2D_add_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MATRIX2D_ASSERT(src_col < src.cols);
    MATRIX2D_ASSERT(des.rows == src.rows);
    MATRIX2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) += MAT2D_AT(src, i, src_col);
    }
}

void mat2D_sub_col_to_col(Mat2D des, size_t des_col, Mat2D src, size_t src_col)
{
    MATRIX2D_ASSERT(src_col < src.cols);
    MATRIX2D_ASSERT(des.rows == src.rows);
    MATRIX2D_ASSERT(des_col < des.cols);

    for (size_t i = 0; i < des.rows; i++) {
        MAT2D_AT(des, i, des_col) -= MAT2D_AT(src, i, src_col);
    }
}

void mat2D_swap_rows(Mat2D m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        double temp = MAT2D_AT(m, r1, j);
        MAT2D_AT(m, r1, j) = MAT2D_AT(m, r2, j);
        MAT2D_AT(m, r2, j) = temp;
    }
}

void mat2D_uint32_swap_rows(Mat2D_uint32 m, size_t r1, size_t r2)
{
    for (size_t j = 0; j < m.cols; j++) {
        double temp = MAT2D_AT_UINT32(m, r1, j);
        MAT2D_AT_UINT32(m, r1, j) = MAT2D_AT_UINT32(m, r2, j);
        MAT2D_AT_UINT32(m, r2, j) = temp;
    }
}

void mat2D_get_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MATRIX2D_ASSERT(src_row < src.rows);
    MATRIX2D_ASSERT(des.cols == src.cols);
    MATRIX2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) = MAT2D_AT(src, src_row, j);
    }
}

void mat2D_add_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MATRIX2D_ASSERT(src_row < src.rows);
    MATRIX2D_ASSERT(des.cols == src.cols);
    MATRIX2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) += MAT2D_AT(src, src_row, j);
    }
}

void mat2D_sub_row_to_row(Mat2D des, size_t des_row, Mat2D src, size_t src_row)
{
    MATRIX2D_ASSERT(src_row < src.rows);
    MATRIX2D_ASSERT(des.cols == src.cols);
    MATRIX2D_ASSERT(des_row < des.rows);

    for (size_t j = 0; j < des.cols; j++) {
        MAT2D_AT(des, des_row, j) -= MAT2D_AT(src, src_row, j);
    }
}

double mat2D_calc_norma(Mat2D m)
{
    double sum = 0;

    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            sum += MAT2D_AT(m, i, j) * MAT2D_AT(m, i, j);
        }
    }
    return sqrt(sum);
}

bool mat2D_mat_is_all_digit(Mat2D m, double digit)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            if (MAT2D_AT(m, i, j) != digit) {
                return false;
            }
        }
    }
    return true;
}

bool mat2D_row_is_all_digit(Mat2D m, double digit, size_t r)
{
    for (size_t j = 0; j < m.cols; ++j) {
        if (MAT2D_AT(m, r, j) != digit) {
            return false;
        }
    }
    return true;
}

bool mat2D_col_is_all_digit(Mat2D m, double digit, size_t c)
{
    for (size_t i = 0; i < m.cols; ++i) {
        if (MAT2D_AT(m, i, c) != digit) {
            return false;
        }
    }
    return true;
}

double mat2D_det_2x2_mat(Mat2D m)
{
    MATRIX2D_ASSERT(2 == m.cols && 2 == m.rows && "Not a 2x2 matrix");
    return MAT2D_AT(m, 0, 0) * MAT2D_AT(m, 1, 1) - MAT2D_AT(m, 0, 1) * MAT2D_AT(m, 1, 0);
}

double mat2D_triangulate(Mat2D m)
{
    /* preforming Gauss elimination: https://en.wikipedia.org/wiki/Gaussian_elimination */
    /* returns the factor multiplying the determinant */

    double factor_to_return = 1;

    for (size_t i = 0; i < (size_t)fmin(m.rows-1, m.cols); i++) {
        if (!MAT2D_AT(m, i, i)) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < m.rows; index++) {
                if (fabs(MAT2D_AT(m, index, i)) > fabs(MAT2D_AT(m, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(m, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < m.cols; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d: [Error] unable to transfrom into uperr triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__);
            }
            double mat_value = MAT2D_AT(m, j, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, mat_value * factor);
        }
    }
    return factor_to_return;
}

double mat2D_det(Mat2D m)
{
    MATRIX2D_ASSERT(m.cols == m.rows && "should be a square matrix");

    /* checking if there is a row or column with all zeros */
    /* checking rows */
    for (size_t i = 0; i < m.rows; i++) {
        if (mat2D_row_is_all_digit(m, 0, i)) {
            return 0;
        }
    }
    /* checking cols */
    for (size_t j = 0; j < m.rows; j++) {
        if (mat2D_col_is_all_digit(m, 0, j)) {
            return 0;
        }
    }

    /* This is an implementation of naive determinant calculation using minors. This is too slow */

    // double det = 0;
    // /* TODO: finding beast row or col? */
    // for (size_t i = 0, j = 0; i < m.rows; i++) { /* first column */
    //     if (MAT2D_AT(m, i, j) < 1e-10) continue;
    //     Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat(m, i, j);
    //     int factor = (i+j)%2 ? -1 : 1;
    //     if (sub_mm.cols != 2) {
    //         MATRIX2D_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
    //         det += MAT2D_AT(m, i, j) * (factor) * mat2D_minor_det(sub_mm);
    //     } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
    //         det += MAT2D_AT(m, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
    //     }
    //     mat2D_minor_free(sub_mm);
    // }

    Mat2D temp_m = mat2D_alloc(m.rows, m.cols);
    mat2D_copy(temp_m, m);
    double factor = mat2D_triangulate(temp_m);
    double diag_mul = 1; 
    for (size_t i = 0; i < temp_m.rows; i++) {
        diag_mul *= MAT2D_AT(temp_m, i, i);
    }
    mat2D_free(temp_m);

    return diag_mul / factor;
}

void mat2D_LUP_decomposition_with_swap(Mat2D src, Mat2D l, Mat2D p, Mat2D u)
{
    /* performing LU decomposition Following the Wikipedia page: https://en.wikipedia.org/wiki/LU_decomposition */

    mat2D_copy(u, src);
    mat2D_set_identity(p);
    mat2D_fill(l, 0);

    for (size_t i = 0; i < (size_t)fmin(u.rows-1, u.cols); i++) {
        if (!MAT2D_AT(u, i, i)) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < u.rows; index++) {
                if (fabs(MAT2D_AT(u, index, i)) > fabs(MAT2D_AT(u, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(u, i, biggest_r);
                mat2D_swap_rows(p, i, biggest_r);
                mat2D_swap_rows(l, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < u.cols; j++) {
            double factor = 1 / MAT2D_AT(u, i, i);
            if (!isfinite(factor)) {
                printf("%s:%d: [Error] unable to transfrom into uper triangular matrix. Probably some of the rows are not independent.\n", __FILE__, __LINE__);
            }
            double mat_value = MAT2D_AT(u, j, i);
            mat2D_sub_row_time_factor_to_row(u, j, i, mat_value * factor);
            MAT2D_AT(l, j, i) = mat_value * factor;
        }
        MAT2D_AT(l, i, i) = 1;
    }
    MAT2D_AT(l, l.rows-1, l.cols-1) = 1;
}

void mat2D_transpose(Mat2D des, Mat2D src)
{
    MATRIX2D_ASSERT(des.cols == src.rows);
    MATRIX2D_ASSERT(des.rows == src.cols);

    for (size_t index = 0; index < des.rows; ++index) {
        for (size_t jndex = 0; jndex < des.cols; ++jndex) {
            MAT2D_AT(des, index, jndex) = MAT2D_AT(src, jndex, index);
        }
    }
}

void mat2D_invert(Mat2D des, Mat2D src)
{
    MATRIX2D_ASSERT(src.cols == src.rows && "should be an NxN matrix");
    MATRIX2D_ASSERT(des.cols == src.cols && des.rows == des.cols);

    Mat2D m = mat2D_alloc(src.rows, src.cols);
    mat2D_copy(m, src);

    mat2D_set_identity(des);
    
    if (!mat2D_det(m)) {
        mat2D_fill(des, 0);
        printf("%s:%d: [Error] Can't invert the matrix. Determinant is zero! Set the inverse matrix to all zeros\n", __FILE__, __LINE__);
        return;
    }

    for (size_t i = 0; i < (size_t)fmin(m.rows-1, m.cols); i++) {
        if (!MAT2D_AT(m, i, i)) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < m.rows; index++) {
                if (fabs(MAT2D_AT(m, index, i)) > fabs(MAT2D_AT(m, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) {
                mat2D_swap_rows(m, i, biggest_r);
                mat2D_swap_rows(des, i, biggest_r);
                printf("%s:%d: [INFO] swapping row %zu with row %zu.\n", __FILE__, __LINE__, i, biggest_r);
            } else {
                MATRIX2D_ASSERT(0 && "can't inverse");
            }
        }
        for (size_t j = i+1; j < m.cols; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            double mat_value = MAT2D_AT(m, j, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, mat_value * factor);
            mat2D_mult_row(m, i, factor);

            mat2D_sub_row_time_factor_to_row(des, j, i, mat_value * factor);
            mat2D_mult_row(des, i, factor);
        }
    }
    double factor = 1 / MAT2D_AT(m, m.rows-1, m.cols-1);
    mat2D_mult_row(m, m.rows-1, factor);
    mat2D_mult_row(des, des.rows-1, factor);
    for (size_t c = m.cols-1; c > 0; c--) {
        for (int r = c-1; r >= 0; r--) {
            double factor = 1 / MAT2D_AT(m, c, c);
            double mat_value = MAT2D_AT(m, r, c);
            mat2D_sub_row_time_factor_to_row(m, r, c, mat_value * factor);
            mat2D_sub_row_time_factor_to_row(des, r, c, mat_value * factor);
        }
    }

    mat2D_free(m);
}

void mat2D_solve_linear_sys_LUP_decomposition(Mat2D A, Mat2D x, Mat2D B)
{
    MATRIX2D_ASSERT(A.cols == x.rows);
    MATRIX2D_ASSERT(1 == x.cols);
    MATRIX2D_ASSERT(A.rows == B.rows);
    MATRIX2D_ASSERT(1 == B.cols);

    Mat2D y     = mat2D_alloc(x.rows, x.cols);
    Mat2D l     = mat2D_alloc(A.rows, A.cols);
    Mat2D p     = mat2D_alloc(A.rows, A.cols);
    Mat2D u     = mat2D_alloc(A.rows, A.cols);
    Mat2D inv_l = mat2D_alloc(l.rows, l.cols);
    Mat2D inv_u = mat2D_alloc(u.rows, u.cols);

    mat2D_LUP_decomposition_with_swap(A, l, p, u);

    mat2D_invert(inv_l, l);
    mat2D_invert(inv_u, u);

    mat2D_fill(x, 0);   /* x here is only a temp mat*/
    mat2D_fill(y, 0);
    mat2D_dot(x, p, B);
    mat2D_dot(y, inv_l, x);

    mat2D_fill(x, 0);
    mat2D_dot(x, inv_u, y);

    mat2D_free(y);
    mat2D_free(l);
    mat2D_free(p);
    mat2D_free(u);
    mat2D_free(inv_l);
    mat2D_free(inv_u);
}

Mat2D_Minor mat2D_minor_alloc_fill_from_mat(Mat2D ref_mat, size_t i, size_t j)
{
    MATRIX2D_ASSERT(ref_mat.cols == ref_mat.rows && "minor is defined only for square matrix");

    Mat2D_Minor mm;
    mm.cols = ref_mat.cols-1;
    mm.rows = ref_mat.rows-1;
    mm.stride_r = ref_mat.cols-1;
    mm.cols_list = (size_t*)MATRIX2D_MALLOC(sizeof(double)*(ref_mat.cols-1));
    mm.rows_list = (size_t*)MATRIX2D_MALLOC(sizeof(double)*(ref_mat.rows-1));
    mm.ref_mat = ref_mat;

    MATRIX2D_ASSERT(mm.cols_list != NULL && mm.rows_list != NULL);

    for (size_t index = 0, temp_index = 0; index < ref_mat.rows; index++) {
        if (index != i) {
            mm.rows_list[temp_index] = index;
            temp_index++;
        }
    }
    for (size_t jndex = 0, temp_jndex = 0; jndex < ref_mat.rows; jndex++) {
        if (jndex != j) {
            mm.cols_list[temp_jndex] = jndex;
            temp_jndex++;
        }
    }

    return mm;
}

Mat2D_Minor mat2D_minor_alloc_fill_from_mat_minor(Mat2D_Minor ref_mm, size_t i, size_t j)
{
    MATRIX2D_ASSERT(ref_mm.cols == ref_mm.rows && "minor is defined only for square matrix");

    Mat2D_Minor mm;
    mm.cols = ref_mm.cols-1;
    mm.rows = ref_mm.rows-1;
    mm.stride_r = ref_mm.cols-1;
    mm.cols_list = (size_t*)MATRIX2D_MALLOC(sizeof(double)*(ref_mm.cols-1));
    mm.rows_list = (size_t*)MATRIX2D_MALLOC(sizeof(double)*(ref_mm.rows-1));
    mm.ref_mat = ref_mm.ref_mat;

    MATRIX2D_ASSERT(mm.cols_list != NULL && mm.rows_list != NULL);

    for (size_t index = 0, temp_index = 0; index < ref_mm.rows; index++) {
        if (index != i) {
            mm.rows_list[temp_index] = ref_mm.rows_list[index];
            temp_index++;
        }
    }
    for (size_t jndex = 0, temp_jndex = 0; jndex < ref_mm.rows; jndex++) {
        if (jndex != j) {
            mm.cols_list[temp_jndex] = ref_mm.cols_list[jndex];
            temp_jndex++;
        }
    }

    return mm;
}

void mat2D_minor_free(Mat2D_Minor mm)
{
    free(mm.cols_list);
    free(mm.rows_list);
}

void mat2D_minor_print(Mat2D_Minor mm, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < mm.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < mm.cols; ++j) {
            printf("%f ", MAT2D_MINOR_AT(mm, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

double mat2D_det_2x2_mat_minor(Mat2D_Minor mm)
{
    MATRIX2D_ASSERT(2 == mm.cols && 2 == mm.rows && "Not a 2x2 matrix");
    return MAT2D_MINOR_AT(mm, 0, 0) * MAT2D_MINOR_AT(mm, 1, 1) - MAT2D_MINOR_AT(mm, 0, 1) * MAT2D_MINOR_AT(mm, 1, 0);
}

double mat2D_minor_det(Mat2D_Minor mm)
{
    MATRIX2D_ASSERT(mm.cols == mm.rows && "should be a square matrix");

    double det = 0;
    /* TODO: finding beast row or col? */
    for (size_t i = 0, j = 0; i < mm.rows; i++) { /* first column */
        if (MAT2D_MINOR_AT(mm, i, j) < 1e-10) continue;
        Mat2D_Minor sub_mm = mat2D_minor_alloc_fill_from_mat_minor(mm, i, j);
        int factor = (i+j)%2 ? -1 : 1;
        if (sub_mm.cols != 2) {
            MATRIX2D_ASSERT(sub_mm.cols == sub_mm.rows && "should be a square matrix");
            det += MAT2D_MINOR_AT(mm, i, j) * (factor) * mat2D_minor_det(sub_mm);
        } else if (sub_mm.cols == 2 && sub_mm.rows == 2) {
            det += MAT2D_MINOR_AT(mm, i, j) * (factor) * mat2D_det_2x2_mat_minor(sub_mm);;
        }
        mat2D_minor_free(sub_mm);
    }
    return det;
}


#endif // MATRIX2D_IMPLEMENTATION