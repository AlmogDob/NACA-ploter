clc; clear; close all

NACA = '23121';
num_of_points = 300;

% ========================

if length(NACA) == 4
    m = str2num(NACA(1))/100;
    p = str2num(NACA(2))/10;
    t = str2num(NACA(3:4))/100;
elseif length(NACA) == 5
    L = str2num(NACA(1));
    P = str2num(NACA(2));
    if P > 5 || P < 1
        fprintf('unsupported NACA: %s (LPSTT)!\n1<= S <=5\n', NACA)
    end
    S = str2num(NACA(3));
    t = str2num(NACA(4:5))/100;    
end


delta_x = 1 / (num_of_points-1);
x        = zeros(num_of_points,1);
dy_c__dx = zeros(num_of_points,1);
y_t      = zeros(num_of_points,1);
theta    = zeros(num_of_points,1);
y_c      = zeros(num_of_points,1);
x_L      = zeros(num_of_points,1);
x_U      = zeros(num_of_points,1);
y_L      = zeros(num_of_points,1);
y_U      = zeros(num_of_points,1);
for i = 0:num_of_points-1
    x(i+1)= delta_x * i;
    y_t(i+1) = 5 * t * (0.2969 * sqrt(x(i+1)) - 0.1260 * x(i+1) - 0.3516 * x(i+1)^2 + 0.2843 * x(i+1)^3 - 0.1036 * x(i+1)^4);
    
    if length(NACA) == 4
        if p == 0 || m == 0
            x_U(i+1) = x(i+1);
            x_L(i+1) = x(i+1);
            y_U(i+1) = y_t(i+1);
            y_L(i+1) = -y_t(i+1);
        else 
            if x(i+1) <= p
                y_c(i+1)      = m / p^2 * (2 * p * x(i+1) - x(i+1)^2);
                dy_c__dx(i+1) = m / p^2 * (p - x(i+1));
            else
                y_c(i+1)      = m / (1 - p)^2 * ((1 - 2 * p) + 2 * p * x(i+1) - x(i+1)^2);
                dy_c__dx(i+1) = 2 * m / (1 - p)^2 * (p - x(i+1));
            end
        end
    elseif length(NACA) == 5
        x_mc = 0.05*P;
        CL_i = 0.15*L;
        if S == 0
            r  = 3.3333*x_mc^3 + 0.7*x_mc^2 + 1.1967*x_mc - 0.0040;
            k1 = 1.5149e6*x_mc^4 - 1.0877e6*x_mc^3 + 2.8646e5*x_mc^2 - 3.2968e4*x_mc + 1.4202e3;

            if x(i+1) <= r
                y_c(i+1) = L/2 * (k1 / 6 * (x(i+1)^3 - 3 * r * x(i+1)^2 + r^2 * (3 - r) * x(i+1)));
                dy_c__dx(i+1) = L/2 * (k1 / 6 * (3 * x(i+1)^2 - 6 * r * x(i+1) + r^2 * (3 - r)));
            else
                y_c(i+1) = L/2 * (k1 * r^3 / 6 * (1 - x(i+1)));
                dy_c__dx(i+1) = - L/2 * k1 * r^3 / 6;
            end
        elseif S == 1
            r   =   10.6667*x_mc^3 -        2*x_mc^2 +   1.7333*x_mc - 0.0340;
            k1  = -2.7973e4*x_mc^3 + 1.7973e4*x_mc^2 - 3.8884e3*x_mc + 289.0760;
            k21 =   85.5280*x_mc^3 -  34.9828*x_mc^2 +   4.8032*x_mc - 0.2153;

            if x(i+1) <= r
                y_c(i+1) = L/2 * (k1 / 6 * ((x(i+1) - r)^3 - k21 * (1 - r)^3 * x(i+1) - r^3 * x(i+1) + r^3));
                dy_c__dx(i+1) = L/2 * (k1 / 6 * (3 * (x(i+1) - r)^2 - k21 * (1 - r)^3 - r^3));
            else
                y_c(i+1) = L/2 * (k1 / 6 * (k21 * (x(i+1) - r)^3 - k21 * (1 - r)^3 * x(i+1) - r^3 * x(i+1) + r^3));
                dy_c__dx(i+1) = L/2 * (k1 / 6 * (3 * k21 * (x(i+1) - r)^2 - k21 * (1 - r)^3 - r^3));
            end
        else
            fprintf('unable to create this NACA: %s, S is only 1 of 0\n', NACA);
        end
    elseif length(NACA) == 6
        fprintf('still not supporting NACA 6 digit\n');
    end
    theta(i+1) = atan(dy_c__dx(i+1));
        
    x_U(i+1) = x(i+1) - y_t(i+1) * sin(theta(i+1));
    x_L(i+1) = x(i+1) + y_t(i+1) * sin(theta(i+1));
    y_U(i+1) = y_c(i+1) + y_t(i+1) * cos(theta(i+1));
    y_L(i+1) = y_c(i+1) - y_t(i+1) * cos(theta(i+1));
end

fig1 = figure('Name','1', 'Position',[900,200,700,500]);
hold all
plot(x_U, y_U)
plot(x, y_c, '--k')
plot(x_L, y_L)
axis equal
grid on
grid minor
box on

