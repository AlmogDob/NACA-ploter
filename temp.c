#define SETUP
#define UPDATE
#define RENDER
#include "./include/display.c"
#include "./include/Matrix2D.h"
#define ALMOG_RENDER_SHAPES_IMPLEMENTATION
#include "./include/Almog_Render_Shapes.h"
#include "./include/Almog_Dynamic_Array.h"

typedef struct {
    size_t length;
    size_t capacity;
    Point* elements;
} ada_point_array;

void create_NACA(ada_point_array *U_points, ada_point_array *L_points, ada_point_array *C_points, const size_t num_of_points, const double factor, const double x_offset, const double y_offset, char *NACA);

ada_point_array U_points, L_points, C_points;

char NACA[5+1];
size_t num_of_points;

void setup(game_state_t *game_state)
{

    // game_state->game_is_running = 0;
    game_state->to_flip_y = 1;
    game_state->to_limit_fps = 0;

    strcpy(NACA, "45016");
    num_of_points = 1000;

    SDL_Color white_color = {.r = 255, .g = 255, .b = 255, .a = 255};
    game_state->text_surface = TTF_RenderText_Solid(game_state->font, NACA, white_color);

    (void)game_state;
}

void update(game_state_t *game_state)
{
    double factor = fmin(game_state->window_h, game_state->window_w) / 1.5;
    double x_offset = game_state->window_w/3;
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

void create_NACA(ada_point_array *U_points, ada_point_array *L_points, ada_point_array *C_points, const size_t num_of_points, const double factor, const double x_offset, const double y_offset, char *NACA)
{
    ada_point_array upper_points, lower_points, center_points;
    ada_init_array(Point, upper_points);
    ada_init_array(Point, lower_points);
    ada_init_array(Point, center_points);

    double delta_x = 1 / ((double)num_of_points - 1);
    size_t NACA_len = strlen(NACA);

    double m = 0, p = 0;
    double L = 0, P = 0, S = 0;
    double t = 0;

    /* parse the NACA string */
    if (NACA_len == 4) {
        m = (NACA[0] - '0') / (double)100;
        p = (NACA[1] - '0') / (double)10;
        t = ((NACA[2] - '0') * 10 + (NACA[3] - '0')) / (double)100;
        if ((m == 0 && p != 0) || (m != 0 && p == 0)) {
            fprintf(stderr, "%s:%d: [ERROR] unsupported NACA: %s (MPxx)\n", __FILE__, __LINE__, NACA);
            exit(1);
        }
        // dprintD(m);
        // dprintD(p);
        // dprintD(t);
    } else if (NACA_len == 5) {
        L = (NACA[0] - '0');
        P = (NACA[1] - '0');
        S = (NACA[2] - '0');
        t = ((NACA[3] - '0') * 10 + (NACA[4] - '0')) / (double)100;
        if (L == 0) {
            fprintf(stderr, "%s:%d: [ERROR] unsupported NACA: %s (LPSxx)!\nL != 0\n", __FILE__, __LINE__, NACA);
            exit(1);
        }
        if (P > 5 || P < 1) {
            fprintf(stderr, "%s:%d: [ERROR] unsupported NACA: %s (LPSxx)!\n1<= S <=5\n", __FILE__, __LINE__, NACA);
            exit(1);
        }
        if (S != 0 && S != 1) {
            fprintf(stderr, "%s:%d: [ERROR] unable to create this NACA: %s, S is only 1 of 0\n", __FILE__, __LINE__, NACA);
            exit(1);
        }

    } else if (NACA_len == 6) {
        fprintf(stderr, "%s:%d: [ERROR] unsupported NACA: %s\nNACA 6 digit not yet implemented\n", __FILE__, __LINE__, NACA);
        exit(1);
    }

    /* calc the points */
    for (size_t i = 0; i < num_of_points; i++) {
        double theta;
        double x   = delta_x * i;
        double y_t = 5 * t * (0.2969 * sqrt(x) - 0.1260 * x - 0.3516 * x * x + 0.2843 * x * x * x - 0.1036 * x * x * x * x);
        double y_c, dy_c__dx;
        Point p_U;
        Point p_L;
        Point p_C;

        if (NACA_len == 4) {
            if (p == 0 || m == 0) {
                y_c      = 0;
                dy_c__dx = 0;
                // p_U.x = x; p_U.y = + y_t; p_U.z = 0;
                // p_L.x = x; p_L.y = - y_t; p_L.z = 0;
                // p_C.x = x; p_C.y = 0    ; p_C.z = 0;
            } else {
                if (x <= p) {
                    y_c      = m / p / p * (2 * p * x - x * x);
                    dy_c__dx = m / p / p * (p - x);
                } else {
                    y_c      = m / (1 - p) / (1 - p) * ((1 - 2 * p) + 2 * p * x - x * x);
                    dy_c__dx = 2 * m / (1 - p) / (1 - p) * (p - x);
                }
            }
        } else if (NACA_len == 5) {
            // double CL_i = 0.15 * L;
            double x_mc = 0.05 * P;
            if (S == 0) {
                double r  = 3.3333 * x_mc * x_mc * x_mc + 0.7 * x_mc * x_mc + 1.1967 * x_mc - 0.0040;
                double k1 = 1.5149e6 * x_mc * x_mc * x_mc * x_mc - 1.0877e6 * x_mc * x_mc * x_mc + 2.8646e5 * x_mc * x_mc - 3.2968e4 * x_mc + 1.4202e3;

                if (x <= r) {
                    y_c      = k1 / 6.0 * (x * x * x - 3 * r * x * x + r * r * (3 - r) * x);
                    dy_c__dx = k1 / 6.0 * (3 * x * x - 6 * r * x + r * r * (3 - r));
                } else {
                    y_c      = k1 * r * r * r / 6.0 * (1 - x);
                    dy_c__dx = - k1 * r * r * r / 6.0;
                }
            } else if (S == 1) {
                double r   = 10.6667 * x_mc * x_mc * x_mc - 2 * x_mc * x_mc + 1.7333 * x_mc - 0.0340;
                double k1  = -2.7973e4 * x_mc * x_mc * x_mc + 1.7973e4 * x_mc * x_mc - 3.8884e3 * x_mc + 289.0760;
                double k21 = 85.5280 * x_mc * x_mc * x_mc - 34.9828 * x_mc * x_mc + 4.8032 * x_mc - 0.2153;

                if (x <= r) {
                    y_c      = k1 / 6.0 * ((x - r) * (x - r) * (x - r)- k21 * (1 - r) * (1 - r) * (1 - r) * x - r * r * r * x + r * r * r);
                    dy_c__dx = k1 / 6.0 * (3 * (x - r) * (x - r) - k21 * (1 - r) * (1 - r) * (1 - r) - r * r * r);
                } else {
                    y_c      = k1 / 6.0 * (k21 * (x - r) * (x - r) * (x - r) - k21 * (1 - r) * (1 - r) * (1 - r) * x - r * r * r * x + r * r * r);
                    dy_c__dx = k1 / 6.0 * (3 * k21 * (x - r) * (x - r) - k21 * (1 - r) * (1 - r) * (1 - r) - r * r * r);
                }
            }
            /* scale for different L number. see report */
            y_c      *= L / 2.0;
            dy_c__dx *= L / 2.0;
        }

        theta = atan(dy_c__dx);
        p_U.x = x - y_t * sin(theta);
        p_L.x = x + y_t * sin(theta);
        p_C.x = x                   ;
        p_C.y = y_c                   ;
        p_U.y = y_c + y_t * cos(theta);
        p_L.y = y_c - y_t * cos(theta);
        p_C.z = 0;
        p_U.z = 0;
        p_L.z = 0;

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
