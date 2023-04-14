#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#define _USE_MATH_DEFINES

#include <cmath>
#include <cassert>
#include <climits>

#include <iostream>
#include <random>

#include <GL/freeglut.h>

#include "agent.hpp"


// Width of window
extern const int WINDOW_WIDTH;
// Height of window
extern const int WINDOW_HEIGHT;
// x position of window
extern const int WINDOW_POS_X;
// y position of window
extern const int WINDOW_POS_Y;
// Delay between frames in microseconds
extern const int DELAY;
// Width of grid
extern const int WIDTH;
// Height of grid
extern const int HEIGHT;
// Stores the state of the grid
extern std::vector<int> IMAGE;
// Stores the chemoattractant values
extern std::vector<double> TRAIL;
// Fraction of cells to populate
extern const double FRAC_CELLS;
// Angle agent rotates by when changing direction
extern const double THETA;
// How far the agent moves
extern const double LENGTH;
// Angle between the agent's sensors
extern const double SENSOR_THETA;
// How far away the sensors are from the agent
extern const double SENSOR_LENGTH;
// How many units of chemoattractant to deposit on the grid
extern const double DEPOSIT;
// Decay factor of chemoattractant evaporation
extern const double EVAPORATION;
// Gaussian diffusion kernel
extern const std::vector<double> DIFFUSION;
// Random number generator
extern std::random_device int_rd;


/**
 * Checks whether a floating-point move is valid.
 *
 * @param old_pos the original position
 * @param new_pos the new position
 * @return        whether the move is valid
 */
bool check_move(std::vector<double> old_pos, std::vector<double> new_pos);

/**
 * Checks the value at a floating-point position.
 * 
 * @param pos a vector representing a position
 * @return    the value at the grid space
 */
double sense(std::vector<double> pos);

/**
 * Attempts to move an agent to a new floating-point position.
 * 
 * @param old_pos the original position
 * @param new_pos the new position
 * @return        whether the move was successful
 */
bool move(std::vector<double> old_pos, std::vector<double> new_pos);

/**
 * Deposits a chemoattractant value at a floating-point position.
 *
 * @param pos a vector representing the position
 * @param val the value to deposit
 */
void deposit(std::vector<double> pos, double val);

/**
 * Convolves a Gaussian diffusion kernel on the chemoattractant grid.
 * 
 * @param kernel a 3x3 diffusion kernel
 */
void diffusion(std::vector<double> kernel);

/**
 * Multiplies chemoattractant values by the decay factor.
 * 
 * @param decay a decay factor
 */
void evaporation(double decay);

/**
 * Populates a vector of agents.
 * 
 * @param n how many agents to initialize
 */
std::vector<Agent *> init_agents(int n);

/**
 * Moves each agent, performs sensing, evaporates chemoattracnt and
 * diffuses chemoattractant.
 * 
 * @param agents a vector of agents
 */
void update(std::vector<Agent *> agents);

/**
 * Draws the grid using GLUT.
 */
void display();


#endif // #ifndef __SIMULATION_HPP__