#include <SLIMEMOLD/simulation.hpp>


/**
 * Generates a random number within a range.
 *
 * @param start the integer to start at
 * @param end   the integer to end at
 * @return      the random number
 */
int random_int(int start, int end) {
    std::mt19937 gen(int_rd());
    std::uniform_int_distribution<int> distr(start, end);
    return distr(gen);
}


/**
 * Given an angle and distance, calculates new position.
 *
 * @param pos      a vector representing an x, y position
 * @param theta    the direction to move
 * @param distance how far to move
 * @return         a vector of the new position
 */
std::vector<double> calc_destination(std::vector<double> pos, double theta, double distance) {
    double x_dest = pos.at(0) + distance * std::cos(theta);
    double y_dest = pos.at(1) + distance * std::sin(theta);
    std::vector<double> dest = {x_dest, y_dest};
    return dest;
}


/**
 * Checks whether a position exceeds the dimensions of the grid.
 *
 * @param pos a vector representing an x, y position
 * @return    whether the position exceeds the grid
 */
bool check_bounds(std::vector<int> pos) {
    int x = pos.at(0);
    int y = pos.at(1);
    return ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT));
}


/**
 * Checks whether a grid space is empty.
 *
 * @param pos a vector representing the grid space
 * @return    whether the grid space is empty
 */
bool check_empty(std::vector<int> pos) {
    return IMAGE.at(WIDTH * pos.at(0) + pos.at(1)) == 0;
}


/**
 * Checks whether a grid space move is valid.
 *
 * @param old_pos the original position
 * @param new_pos the new position
 * @return        whether the move is valid
 */
bool check_move(std::vector<int> old_pos, std::vector<int> new_pos) {
    return ((check_bounds(old_pos) && !check_empty(old_pos)) && (check_bounds(new_pos) && check_empty(new_pos)));
}


/**
 * Rounds a floating point position to the integer grid.
 *
 * @param pos a vector of doubles representing a position
 * @return    a vector of integers representing a grid position
 */
std::vector<int> to_grid(std::vector<double> pos) {
    int x = std::round(pos.at(0));
    int y = std::round(pos.at(1));
    std::vector<int> coord = {x, y};
    return coord;
}


/**
 * Checks whether two floating-point positions map to the same grid space.
 *
 * @param pos1 a vector of doubles representing position 1
 * @param pos2 a vector of doubles representing position 2
 * @return     whether the positions map to the same grid space
 */
bool same(std::vector<double> pos1, std::vector<double> pos2) {
    std::vector<int> pos1i = to_grid(pos1);
    std::vector<int> pos2i = to_grid(pos2);
    int x1 = pos1i.at(0);
    int y1 = pos1i.at(1);
    int x2 = pos2i.at(0);
    int y2 = pos2i.at(1);
    return ((x1 == x2) && (y1 == y2));
}


/**
 * Attempts to place an agent at a grid space.
 *
 * @param pos a vector representing the grid space
 * @return    whether the placement was successful
 */
bool place(std::vector<int> pos) {
    bool result = false;
    if (check_bounds(pos) && check_empty(pos)) {
        IMAGE.at(WIDTH * pos.at(0) + pos.at(1)) = 1;
        result = true;
    }
    return result;
}


/**
 * Checks the value at a grid space.
 *
 * @param pos a vector representing the grid space
 * @return    the value at the grid space
 */
double sense(std::vector<int> pos) {
    double result = 0.0;
    if (check_bounds(pos)) {
        result = TRAIL.at(WIDTH*pos.at(0)+pos.at(1));
    }
    return result;
}


/**
 * Attempts to move an agent to a new grid space.
 *
 * @param old_pos the original position
 * @param new_pos the new position
 * @return        whether the move was successful
 */
bool move(std::vector<int> old_pos, std::vector<int> new_pos) {
    bool result = false;
    if (check_move(old_pos, new_pos)) {
        result = true;
        IMAGE.at(WIDTH * old_pos.at(0) + old_pos.at(1)) = 0;
        IMAGE.at(WIDTH * new_pos.at(0) + new_pos.at(1)) = 1;
    }
    return result;
}


/**
 * Deposits a chemoattractant value at a grid space.
 *
 * @param pos a vector representing the grid space
 * @param val the value to deposit
 */
void deposit(std::vector<int> pos, double val) {
    double old_val = TRAIL.at(WIDTH * pos.at(0) + pos.at(1));
    if (check_bounds(pos) && old_val <= (std::numeric_limits<double>::max() - old_val)) {
        TRAIL.at(WIDTH * pos.at(0) + pos.at(1)) += val;
    }
}


bool check_move(std::vector<double> old_pos, std::vector<double> new_pos) {
    return check_move(to_grid(old_pos), to_grid(new_pos));
}


double sense(std::vector<double> pos) {
    return sense(to_grid(pos));
}


bool move(std::vector<double> old_pos, std::vector<double> new_pos) {
    return move(to_grid(old_pos), to_grid(new_pos));
}


void deposit(std::vector<double> pos, double val) {
    deposit(to_grid(pos), val);
}


void diffusion(std::vector<double> kernel) {
    std::vector<double> NEWTRAIL(WIDTH * HEIGHT, 0.0);
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    int x_idx = i + x - 1;
                    int y_idx = j + y - 1;

                    if (x_idx >= 0 && x_idx < WIDTH && y_idx >= 0 && y_idx < HEIGHT) {
                        double num = TRAIL.at(WIDTH * x_idx + y_idx) * kernel.at(3 * x + y);
                        NEWTRAIL.at(WIDTH * i + j) += num;
                    }
                }
            }
        }
    }
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        TRAIL.at(i) = NEWTRAIL.at(i);
    }
}


void evaporation(double decay) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        TRAIL.at(i) *= decay;
    }
}


std::vector<Agent *> init_agents(int n) {
    std::vector<Agent *> agents;
    for (int i = 0; i < n; i++) {
        Agent *agent = new Agent();
        agents.push_back(agent);
    }
    return agents;
}


void update(std::vector<Agent *> agents) {
    for (auto &agent : agents) {
        agent->move_slime();
    }
    for (auto &agent : agents) {
        agent->sense_slime();
    }
    evaporation(EVAPORATION);
    diffusion(DIFFUSION);
}


void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);         

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            glBegin(GL_POLYGON);
            int val = IMAGE.at(WIDTH * i + (HEIGHT - j - 1));
            // Set filled squares to black, and white otherwise
            if (val == 0) {
                glColor3f(1, 1, 1);
            }
            else {
                glColor3f(0, 0, 0);
            }
            // Define vertices of square to draw
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


Agent::Agent() {
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
