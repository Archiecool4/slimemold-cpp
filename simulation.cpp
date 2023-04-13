#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
// #include <chrono>
// #include <thread>
#include <windows.h>
#include <random>
#include <GL/glut.h>
#include <ctime>
#include <cassert>
#include <climits>

const int WIDTH = 32;
const int HEIGHT = 32;
std::vector<int> IMAGE(WIDTH * HEIGHT, 0);
std::vector<double> TRAIL(WIDTH * HEIGHT, 0.0);
const double THETA = M_PI / 4;
const double LENGTH = 1.0;
const double SENSOR_THETA = M_PI / 4;
const double SENSOR_LENGTH = 3.0;
const double DEPOSIT = 5.0;
const double EVAPORATION = 0.9;
const std::vector<double> DIFFUSION = {1 / 16.0, 1 / 8.0, 1 / 16.0,
                                       1 / 8.0, 1 / 4.0, 1 / 8.0,
                                       1 / 16.0, 1 / 8.0, 1 / 16.0};
std::random_device int_rd;

int random_int(int start, int end);
std::vector<double> calc_destination(std::vector<double> pos, double theta, double distance);
bool check_bounds(std::vector<int> pos);
bool check_move(std::vector<int> old_pos, std::vector<int> new_pos);
bool check_move(std::vector<double> old_pos, std::vector<double> new_pos);
std::vector<int> to_grid(std::vector<double> pos);
bool same(std::vector<double> pos1, std::vector<double> pos2);
bool check_empty(std::vector<int> pos);
bool place(std::vector<int> pos);
double sense(std::vector<int> pos);
double sense(std::vector<double> pos);
bool move(std::vector<int> old_pos, std::vector<int> new_pos);
bool move(std::vector<double> old_pos, std::vector<double> new_pos);
void deposit(std::vector<int> pos, double val);
void deposit(std::vector<double> pos, double val);

class Agent {
    private:
        double x_pos;
        double y_pos;
        double orientation;
    
    public:
        Agent();
        void move_slime();
        void sense_slime();
};


Agent::Agent() {
    double offset = (double) SENSOR_LENGTH - 1.0;
    do {
        x_pos = (double) random_int(SENSOR_LENGTH + 1, WIDTH - SENSOR_LENGTH - 1);
        y_pos = (double) random_int(SENSOR_LENGTH + 1, HEIGHT - SENSOR_LENGTH - 1);
    } while (!check_empty(to_grid(std::vector<double> {x_pos, y_pos})));
    assert(place(to_grid(std::vector<double> {x_pos, y_pos})));
    orientation = (double) random_int(0, 360) * M_PI / 180.0;
}


void Agent::move_slime() {
    std::vector<double> pos = {x_pos, y_pos};
    std::vector<double> dest = calc_destination(pos, orientation, LENGTH);
    if (same(pos, dest)) {
        x_pos = dest.at(0);
        y_pos = dest.at(1);
    }
    else if (move(pos, dest)) {
        x_pos = dest.at(0);
        y_pos = dest.at(1);
        deposit(dest, DEPOSIT);
    }
    else {
        orientation = (double) random_int(0, 360) * M_PI / 180.0;
    }
}


void Agent::sense_slime() {
    std::vector<double> pos = {x_pos, y_pos};
    std::vector<double> left_sensor = calc_destination(pos, orientation - SENSOR_THETA, SENSOR_LENGTH);
    std::vector<double> middle_sensor = calc_destination(pos, orientation, SENSOR_LENGTH);
    std::vector<double> right_sensor = calc_destination(pos, orientation + SENSOR_THETA, SENSOR_LENGTH);

    double left = sense(left_sensor);
    double middle = sense(middle_sensor);
    double right = sense(right_sensor);

    if (middle > left && middle > right) {
        return;
    }
    else if (middle < left && middle < right)  {
        if (std::rand() % 2) {
            orientation += THETA;
        }
        else {
            orientation -= THETA;
        }
    }
    else if (left < right) {
        orientation += THETA;
    }
    else if (right < left) {
        orientation -= THETA;
    }
    else {
        return;
    }
}


int random_int(int start, int end) {
    // std::mt19937 gen(int_rd());
    // std::uniform_int_distribution<int> distr(start, end);
    // return distr(gen);
    return (double) std::rand() / (double(RAND_MAX) / (double) (end - start)) + (double) start;
}

std::vector<double> calc_destination(std::vector<double> pos, double theta, double distance) {
    double x_dest = pos.at(0) + distance * std::cos(theta);
    double y_dest = pos.at(1) + distance * std::sin(theta);
    std::vector<double> dest = {x_dest, y_dest};
    return dest;
}


bool check_bounds(std::vector<int> pos) {
    int x = pos.at(0);
    int y = pos.at(1);
    return ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT));
}


bool check_move(std::vector<int> old_pos, std::vector<int> new_pos) {
    return ((check_bounds(old_pos) && !check_empty(old_pos)) && (check_bounds(new_pos) && check_empty(new_pos)));
}

bool check_move(std::vector<double> old_pos, std::vector<double> new_pos) {
    return check_move(to_grid(old_pos), to_grid(new_pos));
}

std::vector<int> to_grid(std::vector<double> pos) {
    int x = std::round(pos.at(0));
    int y = std::round(pos.at(1));
    std::vector<int> coord = {x, y};
    return coord;
}


bool same(std::vector<double> pos1, std::vector<double> pos2) {
    std::vector<int> pos1i = to_grid(pos1);
    std::vector<int> pos2i = to_grid(pos2);
    int x1 = pos1i.at(0);
    int y1 = pos1i.at(1);
    int x2 = pos2i.at(0);
    int y2 = pos2i.at(1);
    return ((x1 == x2) && (y1 == y2));
}


bool check_empty(std::vector<int> pos) {
    return IMAGE.at(WIDTH * pos.at(0) + pos.at(1)) == 0;
}


bool place(std::vector<int> pos) {
    bool result = false;
    if (check_bounds(pos) && check_empty(pos)) {
        IMAGE.at(WIDTH * pos.at(0) + pos.at(1)) = 1;
        result = true;
    }
    return result;
}


double sense(std::vector<int> pos) {
    double result = 0.0;
    if (check_bounds(pos)) {
        result = TRAIL.at(WIDTH*pos.at(0)+pos.at(1));
    }
    return result;
}


double sense(std::vector<double> pos) {
    return sense(to_grid(pos));
}


bool move(std::vector<int> old_pos, std::vector<int> new_pos) {
    bool result = false;
    // std::cout << check_move(old_pos, new_pos) << std::endl;
    if (check_move(old_pos, new_pos)) {
        result = true;
        IMAGE.at(WIDTH * old_pos.at(0) + old_pos.at(1)) = 0;
        IMAGE.at(WIDTH * new_pos.at(0) + new_pos.at(1)) = 1;
    }
    return result;
}


bool move(std::vector<double> old_pos, std::vector<double> new_pos) {
    return move(to_grid(old_pos), to_grid(new_pos));
}


void deposit(std::vector<int> pos, double val) {
    double old_val = TRAIL.at(WIDTH * pos.at(0) + pos.at(1));
    if (check_bounds(pos) && old_val <= (std::numeric_limits<double>::max() - old_val)) {
        TRAIL.at(WIDTH * pos.at(0) + pos.at(1)) += val;
    }
}


void deposit(std::vector<double> pos, double val) {
    deposit(to_grid(pos), val);
}


void diffusion() {
    std::vector<double> NEWTRAIL(WIDTH*HEIGHT, 0.0);
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    int x_idx = i + x - 1;
                    int y_idx = j + y - 1;

                    if (x_idx >= 0 && x_idx < WIDTH && y_idx >= 0 && y_idx < HEIGHT) {
                        double num = TRAIL.at(WIDTH * x_idx + y_idx) * DIFFUSION.at(3 * x + y);
                        NEWTRAIL.at(WIDTH*i+j) += num;
                    }
                }
            }
        }
    }
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        TRAIL.at(i) = NEWTRAIL.at(i);
    }
}


void evaporation() {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        TRAIL.at(i) *= EVAPORATION;
    }
}

std::vector<Agent> init(int n) {
    std::vector<Agent> agents;
    for (int i = 0; i < n; i++) {
        agents.push_back(*(new Agent{}));
    }
    return agents;
}

void update(std::vector<Agent> agents) {
    for (auto &agent : agents) {
        agent.move_slime();
    }
    for (auto &agent : agents) {
        agent.sense_slime();
    }
    evaporation();
    diffusion();
}

void print() {
    system("cls");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            int val = IMAGE.at(WIDTH*i+j);
            // double val = TRAIL.at(WIDTH*i+j);
            std::cout << val << " ";
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}


void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set backgstd::round color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            glBegin(GL_POLYGON);
            // double max = -1.0;
            // for (int k = 0; k < WIDTH * HEIGHT; k++) {
            //     if (TRAIL[k] > max) {
            //         max = TRAIL.at(k);
            //     }
            // }
            // double val = TRAIL.at(WIDTH*i+(HEIGHT-j-1));
            // glColor3f(val / max, 0, 0);
            int val = IMAGE.at(WIDTH*i+(HEIGHT-j-1));
            if (val == 0) {
                glColor3f(1, 1, 1);
            }
            else {
                glColor3f(0, 0, 0);
            }
            double x_scale = (double) WIDTH / 2;
            double y_scale = (double) HEIGHT / 2;
            double x = -1 + i / x_scale, y = -1 + j / y_scale;
            double x_inc = 1 / x_scale;
            double y_inc = 1 / y_scale;
            glVertex2f(x, y);
            glVertex2f(x + x_inc, y);
            glVertex2f(x + x_inc, y + y_inc);
            glVertex2f(x, y + y_inc);
            glEnd();
        }
    }
    glFlush();
}


int main(int argc, char *argv[]) {
    std::srand(time(0));
    std::vector<Agent> agents = init(int(WIDTH * HEIGHT * 0.1));

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Slime Mold Simulation"); // Create a window with the given title
    glutInitWindowSize(500, 500);              // Set the window's initial width & height
    glutInitWindowPosition(80, 80);            // Position the window's initial top-left corner
    glutDisplayFunc(display);                  // Register display callback handler for window re-paint

    while (true) {
        update(agents);
        // print();
        display();
        Sleep(200);
    }

    for (auto &agent : agents) {
        delete &agent;
    }
}