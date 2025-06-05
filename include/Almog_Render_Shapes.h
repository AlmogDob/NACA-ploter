#ifndef ALMOG_RENDER_SHAPES_H_
#define ALMOG_RENDER_SHAPES_H_

#include <math.h>
#include <stdint.h>
#include "Matrix2D.h"

#ifndef POINT
#define POINT
typedef struct {
    float x;
    float y;
    float z;
} Point ;
#endif

#ifndef TRI
#define TRI
typedef struct {
    Point points[3];
    Point center;
    float z_min;
    float z_max;
    bool to_draw;
    float light_intensity;
} Tri;
#endif

#ifndef MESH
#define MESH
typedef struct {
    size_t length;
    size_t capacity;
    Tri *elements;
} Mesh; /* Tri ada array */
#endif

#ifndef HexARGB_RGBA
#define HexARGB_RGBA(x) (x>>(8*2)&0xFF), (x>>(8*1)&0xFF), (x>>(8*0)&0xFF), (x>>(8*3)&0xFF)
#endif
#ifndef HexARGB_RGBA_VAR
#define HexARGB_RGBA_VAR(x) uint8_t r = (x>>(8*2)&0xFF); uint8_t g = (x>>(8*1)&0xFF); uint8_t b = (x>>(8*0)&0xFF); uint8_t a = (x>>(8*3)&0xFF)
#endif
#ifndef ARGB_hexARGB
#define ARGB_hexARGB(a, r, g, b) 0x01000000*(a) + 0x00010000*(r) + 0x00000100*(g) + 0x00000001*(b)
#endif
#ifndef RGB_hexRGB
#define RGB_hexRGB(r, g, b) (int)(0x010000*(r) + 0x000100*(g) + 0x000001*(b))
#endif

#define edge_cross_point(a1, b, a2, p) (b.x-a1.x)*(p.y-a2.y)-(b.y-a1.y)*(p.x-a2.x)
#define is_top_edge(x, y) (y == 0 && x > 0)
#define is_left_edge(x, y) (y < 0)
#define is_top_left(ps, pe) (is_top_edge(pe.x-ps.x, pe.y-ps.y) || is_left_edge(pe.x-ps.x, pe.y-ps.y))

void ars_draw_point(Mat2D_uint32 screen_mat, int x, int y, uint32_t color);
void ars_draw_line(Mat2D_uint32 screen_mat, int x1, int y1, int x2, int y2, uint32_t color);
void ars_draw_lines(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color);
void ars_draw_lines_loop(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color);
void ars_draw_arrow(Mat2D_uint32 screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color);


void ars_draw_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color);
void ars_fill_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color);

void ars_draw_tri(Mat2D_uint32 screen_mat, Tri tri, uint32_t color);
void ars_fill_tri_scanline_rasterizer(Mat2D_uint32 screen_mat, Tri tri, uint32_t color);
void ars_fill_tri_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Tri tri, uint32_t color, float light_intensity);
void ars_fill_tri_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Tri tri, uint32_t color);
void ars_fill_tri_Pinedas_rasterizer_with_mat2D(Mat2D_uint32 screen_mat, Tri tri, uint32_t color);

void ars_draw_mesh(Mat2D_uint32 screen_mat, Mesh mesh, uint32_t color);
void ars_fill_mesh_scanline_rasterizer(Mat2D_uint32 screen_mat, Mesh mesh, uint32_t color);
void ars_fill_mesh_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mesh mesh, int color);

#endif /*ALMOG_RENDER_SHAPES_H_*/

#ifdef ALMOG_RENDER_SHAPES_IMPLEMENTATION
#undef ALMOG_RENDER_SHAPES_IMPLEMENTATION

void ars_draw_point(Mat2D_uint32 screen_mat, int x, int y, uint32_t color)
{
    if ((x < (int)screen_mat.cols && y < (int)screen_mat.rows) && (x >= 0 && y >= 0)) { /* point is in screen */
        MAT2D_AT_UINT32(screen_mat, y, x) = color;
    }
}

void ars_draw_line(Mat2D_uint32 screen_mat, int x1, int y1, int x2, int y2, uint32_t color)
{
    /* This function is inspired by the Olive.c function developed by 'Tsoding' on his YouTube channel. You can fined the video in this link: https://youtu.be/LmQKZmQh1ZQ?list=PLpM-Dvs8t0Va-Gb0Dp4d9t8yvNFHaKH6N&t=4683. */

    int x = x1;
    int y = y1;
    int dx, dy;

    ars_draw_point(screen_mat, x, y, color);

    dx = x2 - x1;
    dy = y2 - y1;

    if (0 == dx && 0 == dy) return;
    if (0 == dx) {
        while (x != x2 || y != y2) {
            if (dy > 0) {
                y++;
            }
            if (dy < 0) {
                y--;
            }
            ars_draw_point(screen_mat, x, y, color);
        }
        return;
    }
    if (0 == dy) {
        while (x != x2 || y != y2) {
            if (dx > 0) {
                x++;
            }
            if (dx < 0) {
                x--;
            }
            ars_draw_point(screen_mat, x, y, color);
        }
        return;
    }

    /* float m = (float)dy / dx */
    int b = y1 - dy * x1 / dx;

    if (x1 > x2) {
        int temp_x = x1;
        x1 = x2;
        x2 = temp_x;
    }
    for (x = x1; x < x2; x++) {
        int sy1 = dy * x / dx + b;
        int sy2 = dy * (x + 1) / dx + b;
        if (sy1 > sy2) {
            int temp_y = sy1;
            sy1 = sy2;
            sy2 = temp_y;
        }
        for (y = sy1; y <= sy2; y++) {
            ars_draw_point(screen_mat, x, y, color);
        }
    }

}

void ars_draw_lines(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color) 
{
    for (size_t i = 0; i < len-1; i++) {
        ars_draw_line(screen_mat, points[i].x, points[i].y, points[i+1].x, points[i+1].y, color);
    }
}

void ars_draw_lines_loop(const Mat2D_uint32 screen_mat, const Point *points, const size_t len, const uint32_t color)
{
    for (size_t i = 0; i < len-1; i++) {
        ars_draw_line(screen_mat, points[i].x, points[i].y, points[i+1].x, points[i+1].y, color);
    }
    ars_draw_line(screen_mat, points[len-1].x, points[len-1].y, points[0].x, points[0].y, color);
}

/* This function is a bit complicated and expansive but this is what I could come up with */
void ars_draw_arrow(Mat2D_uint32 screen_mat, int xs, int ys, int xe, int ye, float head_size, float angle_deg, uint32_t color)
{
    Mat2D pe = mat2D_alloc(3, 1);
    mat2D_fill(pe, 0);
    MAT2D_AT(pe, 0, 0) = xe;
    MAT2D_AT(pe, 1, 0) = ye;
    Mat2D v1 = mat2D_alloc(3, 1);
    mat2D_fill(v1, 0);
    Mat2D v2 = mat2D_alloc(3, 1);
    mat2D_fill(v2, 0);
    Mat2D temp_v = mat2D_alloc(3, 1);
    mat2D_fill(temp_v, 0);
    Mat2D DCM_p = mat2D_alloc(3, 3);
    mat2D_fill(DCM_p, 0);
    mat2D_set_rot_mat_z(DCM_p, angle_deg);
    Mat2D DCM_m = mat2D_alloc(3, 3);
    mat2D_fill(DCM_m, 0);
    mat2D_set_rot_mat_z(DCM_m, -angle_deg);

    int x_center = xs*head_size + xe*(1-head_size);
    int y_center = ys*head_size + ye*(1-head_size);

    MAT2D_AT(v1, 0, 0) = x_center;
    MAT2D_AT(v1, 1, 0) = y_center;
    mat2D_copy(v2, v1);

    /* v1 */
    mat2D_copy(temp_v, v1);
    mat2D_sub(temp_v, pe);
    mat2D_fill(v1, 0);
    mat2D_dot(v1, DCM_p, temp_v);
    mat2D_add(v1, pe);

    /* v2 */
    mat2D_copy(temp_v, v2);
    mat2D_sub(temp_v, pe);
    mat2D_fill(v2, 0);
    mat2D_dot(v2, DCM_m, temp_v);
    mat2D_add(v2, pe);

    ars_draw_line(screen_mat, MAT2D_AT(v1, 0, 0), MAT2D_AT(v1, 1, 0), xe, ye, color);
    ars_draw_line(screen_mat, MAT2D_AT(v2, 0, 0), MAT2D_AT(v2, 1, 0), xe, ye, color);
    ars_draw_line(screen_mat, xs, ys, xe, ye, color);

    mat2D_free(pe);
    mat2D_free(v1);
    mat2D_free(v2);
    mat2D_free(temp_v);
    mat2D_free(DCM_p);
    mat2D_free(DCM_m);
}

void ars_point_to_mat2D(Point p, Mat2D m)
{
    MATRIX2D_ASSERT((3 == m.rows && 1 == m.cols) || (1 == m.rows && 3 == m.cols));
    
    if (3 == m.rows) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 1, 0) = p.y;
        MAT2D_AT(m, 2, 0) = p.z;
    }
    if (3 == m.cols) {
        MAT2D_AT(m, 0, 0) = p.x;
        MAT2D_AT(m, 0, 1) = p.y;
        MAT2D_AT(m, 0, 2) = p.z;
    }

}

void ars_draw_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0 && diff > -r*2) {
                ars_draw_point(screen_mat, center_x + dx, center_y + dy, color);
            }
        }
    }
}

void ars_fill_circle(Mat2D_uint32 screen_mat, float center_x, float center_y, float r, uint32_t color)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            float diff = dx * dx + dy * dy - r * r;
            if (diff < 0) {
                ars_draw_point(screen_mat, center_x + dx, center_y + dy, color);
            }
        }
    }
}

void ars_draw_tri(Mat2D_uint32 screen_mat, Tri tri, uint32_t color)
{
    ars_draw_line(screen_mat, tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, color);
    ars_draw_line(screen_mat, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, color);
    ars_draw_line(screen_mat, tri.points[2].x, tri.points[2].y, tri.points[0].x, tri.points[0].y, color);
}

    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

/* This works but there are some artifacts */
void ars_fill_tri_scanline_rasterizer(Mat2D_uint32 screen_mat, Tri tri, uint32_t color)
{
    /* arranging the points according to y value */
    Point p0 = tri.points[0];
    Point p1 = tri.points[1];
    Point p2 = tri.points[2];
    if (p1.y > p0.y) {
        Point temp = p1;
        p1 = p0;
        p0 = temp;
    }
    if (p2.y > p1.y) {
        Point temp = p2;
        p2 = p1;
        p1 = temp;
        if (p1.y > p0.y) {
            Point temp = p1;
            p1 = p0;
            p0 = temp;
        }
    }
    if (p2.y > p0.y) {
        Point temp = p2;
        p2 = p0;
        p0 = temp;
    }

    /* finding max and min x */
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));

    if (p0.x == p1.x && p1.x == p2.x) {
        ars_draw_tri(screen_mat, tri, color);
        return;
    }

    /* The rasterization */
    float m01 = (p0.y - p1.y) / (p0.x - p1.x);
    float b01 = p0.y - m01 * p0.x;
    float m02 = (p0.y - p2.y) / (p0.x - p2.x);
    float b02 = p0.y - m02 * p0.x;
    float m12 = (p1.y - p2.y) / (p1.x - p2.x);
    float b12 = p1.y - m12 * p1.x;

    float epsilon = 1e-3;
    int gap = 15;
    // printf("m01: %f, m02: %f, m12: %f\n", m01, m02, m12);
    if (fabs(m02) < epsilon || fabs(m12) < epsilon || fabs(m01) < epsilon) return;
    for (int y = (int)p2.y; y < (int)p1.y; y++)
    {
        float x02 = (y - b02) / m02;
        float x12 = (y - b12) / m12;
        if (x02 <= x_min-gap || x02 >= x_max+gap) continue;
        if (x12 <= x_min-gap || x12 >= x_max+gap) continue;
        if (fabs(p0.x - p2.x) - fabs(p0.x - x02) < 0) continue;
        if (fabs(p1.x - p2.x) - fabs(p1.x - x12) < 0) continue;
        ars_draw_line(screen_mat, x02, y, x12, y, color);
        // printf("x02: %d, x12: %d, y: %d\n", (int)x02, (int)x12, (int)y);
    }
    for (int y = (int)p1.y; y <= (int)p0.y; y++) {
        float x01 = (y - b01) / m01;
        float x02 = (y - b02) / m02;
        if (x01 <= x_min-gap || x01 >= x_max+gap) continue;
        if (x02 <= x_min-gap || x02 >= x_max+gap) continue;
        if (fabs(p1.x - p0.x) - fabs(p1.x - x01) < 0) continue;
        if (fabs(p0.x - p2.x) - fabs(p0.x - x02) < 0) continue;
        ars_draw_line(screen_mat, x02, y, x01, y, color);
    }
}

/* This function is the function for rasterization */
void ars_fill_tri_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Tri tri, uint32_t color, float light_intensity)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

    Point p0, p1, p2;
    p0 = tri.points[0];
    p1 = tri.points[1];
    p2 = tri.points[2];

    float w = edge_cross_point(p0, p1, p1, p2);
    if (!w) {
        ars_draw_tri(screen_mat, tri, color);
        return;
    }
    MATRIX2D_ASSERT(w != 0 && "triangle has area");

    /* fill conventions */
    int bias0 = is_top_left(p0, p1) ? 0 : -1;
    int bias1 = is_top_left(p1, p2) ? 0 : -1;
    int bias2 = is_top_left(p2, p0) ? 0 : -1;

    /* finding bounding box */
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int y_min = fmin(p0.y, fmin(p1.y, p2.y));
    int y_max = fmax(p0.y, fmax(p1.y, p2.y));
    // printf("xmin: %d, xmax: %d || ymin: %d, ymax: %d\n", x_min, x_max, y_min, y_max);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            // ars_draw_point(screen_mat, x, y, color);
            Point p = {.x = x, .y = y, .z = 0};

            float w0 = edge_cross_point(p0, p1, p0, p) + bias0;
            float w1 = edge_cross_point(p1, p2, p1, p) + bias1;
            float w2 = edge_cross_point(p2, p0, p2, p) + bias2;

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
                HexARGB_RGBA_VAR(color);
                r *= light_intensity;
                g *= light_intensity;
                b *= light_intensity;
                (void)a;
                ars_draw_point(screen_mat, x, y, RGB_hexRGB(r, g, b));
            }
        }
    }
}

void ars_fill_tri_Pinedas_rasterizer_interpolate_color(Mat2D_uint32 screen_mat, Tri tri, uint32_t color)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */

    Point p0, p1, p2;
    p0 = tri.points[0];
    p1 = tri.points[1];
    p2 = tri.points[2];

    float w = edge_cross_point(p0, p1, p1, p2);
    MATRIX2D_ASSERT(w != 0 && "triangle has area");

    /* fill conventions */
    int bias0 = is_top_left(p0, p1) ? 0 : -1;
    int bias1 = is_top_left(p1, p2) ? 0 : -1;
    int bias2 = is_top_left(p2, p0) ? 0 : -1;

    /* finding bounding box */
    int x_min = fmin(p0.x, fmin(p1.x, p2.x));
    int x_max = fmax(p0.x, fmax(p1.x, p2.x));
    int y_min = fmin(p0.y, fmin(p1.y, p2.y));
    int y_max = fmax(p0.y, fmax(p1.y, p2.y));
    // printf("xmin: %d, xmax: %d || ymin: %d, ymax: %d\n", x_min, x_max, y_min, y_max);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            // ars_draw_point(screen_mat, x, y, color);
            Point p = {.x = x, .y = y, .z = 0};

            float w0 = edge_cross_point(p0, p1, p0, p) + bias0;
            float w1 = edge_cross_point(p1, p2, p1, p) + bias1;
            float w2 = edge_cross_point(p2, p0, p2, p) + bias2;

            float alpha = fabs(w0 / w);
            float beta  = fabs(w1 / w);
            float gamma = fabs(w2 / w);
            // printf("alpha: %5f, beta: %5f, gamma: %5f\n", alpha, beta, gamma);

            if (w0 * w >= 0 && w1 * w >= 0 &&  w2 * w >= 0) {
            // if (w0 < 0 && w1 < 0 &&  w2 < 0) {
                HexARGB_RGBA_VAR(color);
                
                uint8_t current_r = r*alpha + 0xaa*beta + 0x22*gamma;
                uint8_t current_g = g*alpha + 0xaa*beta + 0x22*gamma;
                uint8_t current_b = b*alpha + 0xaa*beta + 0x22*gamma;

                ars_draw_point(screen_mat, x, y, ARGB_hexARGB(a, current_r, current_g, current_b));
            }
        }
    }
}

/* Works good but very slow! (the dynamic allocation is bad for performance)*/
void ars_fill_tri_Pinedas_rasterizer_with_mat2D(Mat2D_uint32 screen_mat, Tri tri, uint32_t color)
{
    /* This function follows the rasterizer of 'Pikuma' shown in his YouTube video. You can fine the video in this link: https://youtu.be/k5wtuKWmV48. */
    Mat2D p0       = mat2D_alloc(3, 1);
    Mat2D p1       = mat2D_alloc(3, 1);
    Mat2D p2       = mat2D_alloc(3, 1);
    Mat2D v01      = mat2D_alloc(3, 1);
    Mat2D v12      = mat2D_alloc(3, 1);
    Mat2D v20      = mat2D_alloc(3, 1);
    Mat2D p        = mat2D_alloc(3, 1);
    Mat2D v0p      = mat2D_alloc(3, 1);
    Mat2D v1p      = mat2D_alloc(3, 1);
    Mat2D v2p      = mat2D_alloc(3, 1);
    Mat2D cross012 = mat2D_alloc(3, 1);
    Mat2D cross01p = mat2D_alloc(3, 1);
    Mat2D cross12p = mat2D_alloc(3, 1);
    Mat2D cross20p = mat2D_alloc(3, 1);

    ars_point_to_mat2D(tri.points[0], p0);
    ars_point_to_mat2D(tri.points[1], p1);
    ars_point_to_mat2D(tri.points[2], p2);

    mat2D_copy(v01, p1);
    mat2D_sub(v01, p0);
    mat2D_copy(v12, p2);
    mat2D_sub(v12, p1);
    mat2D_copy(v20, p0);
    mat2D_sub(v20, p2);

    mat2D_cross(cross012, v01, v12);
    float w = MAT2D_AT(cross012, 2, 0);

    /* finding bounding box */
    int x_min = fmin(MAT2D_AT(p0, 0, 0), fmin(MAT2D_AT(p1, 0, 0), MAT2D_AT(p2, 0, 0)));
    int x_max = fmax(MAT2D_AT(p0, 0, 0), fmax(MAT2D_AT(p1, 0, 0), MAT2D_AT(p2, 0, 0)));
    int y_min = fmin(MAT2D_AT(p0, 1, 0), fmin(MAT2D_AT(p1, 1, 0), MAT2D_AT(p2, 1, 0)));
    int y_max = fmax(MAT2D_AT(p0, 1, 0), fmax(MAT2D_AT(p1, 1, 0), MAT2D_AT(p2, 1, 0)));
    // printf("xmin: %d, xmax: %d || ymin: %d, ymax: %d\n", x_min, x_max, y_min, y_max);

    for (int y = y_min; y <= y_max; y++) {
        for (int x = x_min; x <= x_max; x++) {
            // ars_draw_point(screen_mat, x, y, color);
            MAT2D_AT(p, 0, 0) = x;
            MAT2D_AT(p, 1, 0) = y;
            MAT2D_AT(p, 2, 0) = 0;

            mat2D_copy(v0p, p);
            mat2D_sub(v0p, p0);
            mat2D_copy(v1p, p);
            mat2D_sub(v1p, p1);
            mat2D_copy(v2p, p);
            mat2D_sub(v2p, p2);

            mat2D_cross(cross01p, v01, v0p);
            mat2D_cross(cross12p, v12, v1p);
            mat2D_cross(cross20p, v20, v2p);

            float w0 = MAT2D_AT(cross01p, 2, 0);
            float w1 = MAT2D_AT(cross12p, 2, 0);
            float w2 = MAT2D_AT(cross20p, 2, 0);

            if (w0 * w > 0 && w1 * w > 0 &&  w2 * w > 0) {
                ars_draw_point(screen_mat, x, y, color);
            }
        }
    }

    mat2D_free(p0);   
    mat2D_free(p1);   
    mat2D_free(p2);   
    mat2D_free(v01);   
    mat2D_free(v12);   
    mat2D_free(v20);   
    mat2D_free(p);   
    mat2D_free(v0p);   
    mat2D_free(v1p);   
    mat2D_free(v2p);   
    mat2D_free(cross012);   
    mat2D_free(cross01p);   
    mat2D_free(cross12p);   
    mat2D_free(cross20p);   
}

void ars_draw_mesh(Mat2D_uint32 screen_mat, Mesh mesh, uint32_t color)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            ars_draw_tri(screen_mat, tri, color);
        }
    }


}

void ars_fill_mesh_scanline_rasterizer(Mat2D_uint32 screen_mat, Mesh mesh, uint32_t color)
{
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (tri.to_draw) {
            ars_fill_tri_scanline_rasterizer(screen_mat, tri, color);
        }
    }
}

void ars_fill_mesh_Pinedas_rasterizer(Mat2D_uint32 screen_mat, Mesh mesh, int color)
{
    bool rand_color;
    if (-1 == color) {
        rand_color = true;
    }
    for (size_t i = 0; i < mesh.length; i++) {
        Tri tri = mesh.elements[i];
        if (rand_color) {
            color = rand_double() * 0xFFFFFF;
        }
        if (tri.to_draw) {
            ars_fill_tri_Pinedas_rasterizer(screen_mat, tri, color, tri.light_intensity);
        }
    }
}

#endif /*ALMOG_RENDER_SHAPES_IMPLEMENTATION*/
