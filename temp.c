#define SETUP
#define UPDATE
#define RENDER
#include "display.c"
#include "Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "Almog_Render_Shapes.h"
#include "Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    Point* elements;
} ada_point_array;

void create_NACA(ada_point_array *U_points, ada_point_array *L_points, ada_point_array *C_points, const size_t num_of_points, const double factor, const double x_offset, const double y_offset, int NACA);

ada_point_array U_points, L_points, C_points;

void setup(game_state_t *game_state)
{

    // game_state->game_is_running = 0;
    game_state->to_flip_y = 1;
    game_state->to_limit_fps = 0;

    (void)game_state;
}

void update(game_state_t *game_state)
{
    size_t num_of_points = 1000;
    int NACA = 6450;
    double factor = fmin(game_state->window_h, game_state->window_w) / 1.5;
    double x_offset = game_state->window_w/4;
    double y_offset = game_state->window_h/2;

    create_NACA(&U_points, &L_points, &C_points, num_of_points, factor, x_offset, y_offset, NACA);
}

void render(game_state_t *game_state)
{
    ars_draw_lines(game_state->window_pixels_mat, U_points.elements, U_points.length, 0xFF00FF);
    ars_draw_lines(game_state->window_pixels_mat, L_points.elements, L_points.length, 0xFFFF00);
    ars_draw_lines(game_state->window_pixels_mat, C_points.elements, C_points.length, 0xFFFFFF);

    free(U_points.elements);
    free(L_points.elements);
    free(C_points.elements);
}

void create_NACA(ada_point_array *U_points, ada_point_array *L_points, ada_point_array *C_points, const size_t num_of_points, const double factor, const double x_offset, const double y_offset, int NACA)
{
    ada_point_array upper_points, lower_points, center_points;
    ada_init_array(Point, upper_points);
    ada_init_array(Point, lower_points);
    ada_init_array(Point, center_points);

    double m = ((NACA % 10000)/1000) / (double)100;
    double p = ((NACA % 1000)/100) / (double)10;
    double t = (NACA % 100) / (double)100;

    double delta_x = 1 / ((double)num_of_points - 1);
    for (size_t i = 0; i < num_of_points; i++) {
        double x   = delta_x * i;
        double y_t = 5 * t * (0.2969 * sqrt(x) - 0.1260 * x - 0.3516 * x * x + 0.2843 * x * x * x - 0.1036 * x * x * x * x);
        double y_c, dy_c__dx;
        if (x <= p) {
            y_c      = m / p / p * (2 * p * x - x * x);
            dy_c__dx = m / p / p * (p - x);
        } else {
            y_c      = m / (1 - p) / (1 - p) * ((1 - 2 * p) + 2 * p * x - x * x);
            dy_c__dx = 2 * m / (1 - p) / (1 - p) * (p - x);
        }
        double theta = atan(dy_c__dx);
        Point p_U = {.x = x - y_t * sin(theta), .y = y_c + y_t * cos(theta), .z = 0};
        Point p_L = {.x = x + y_t * sin(theta), .y = y_c - y_t * cos(theta), .z = 0};
        Point p_C = {.x = x, .y = y_c, .z = 0};

        p_U.x = p_U.x * factor + x_offset;
        p_L.x = p_L.x * factor + x_offset;
        p_C.x = p_C.x * factor + x_offset;

        p_U.y = p_U.y * factor + y_offset;
        p_L.y = p_L.y * factor + y_offset;
        p_C.y = p_C.y * factor + y_offset;

        // printf("p_U: (%f, %f, %f)\n", p_U.x, p_U.y, p_U.z);
        // printf("p_L: (%f, %f, %f)\n", p_L.x, p_L.y, p_L.z);
        // printf("p_C: (%f, %f, %f)\n", p_C.x, p_C.y, p_C.z);

        ada_appand(Point, upper_points, p_U);
        ada_appand(Point, lower_points, p_L);
        ada_appand(Point, center_points, p_C);
    }
    U_points->capacity = upper_points.capacity;
    U_points->elements = upper_points.elements;
    U_points->length   = upper_points.length;

    L_points->capacity = lower_points.capacity;
    L_points->elements = lower_points.elements;
    L_points->length   = lower_points.length;

    C_points->capacity = center_points.capacity;
    C_points->elements = center_points.elements;
    C_points->length   = center_points.length;
}
