#include <unistd.h>

#include <SLIMEMOLD/simulation.hpp>


// Define simulation parameters

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int WINDOW_POS_X = 80;
const int WINDOW_POS_Y = 80;
const int DELAY = 1000;
const int WIDTH = 256;
const int HEIGHT = 256;
std::vector<int> IMAGE(WIDTH * HEIGHT, 0);
std::vector<double> TRAIL(WIDTH * HEIGHT, 0.0);
const double FRAC_CELLS = 0.1;
const double THETA = M_PI / 4;
const double LENGTH = 1.0;
const double SENSOR_THETA = M_PI / 4;
const double SENSOR_LENGTH = 3.0;
const double DEPOSIT = 5.0;
const double EVAPORATION = 0.9;
const std::vector<double> DIFFUSION = { 1 / 16.0, 1 / 8.0, 1 / 16.0,
                                        1 / 8.0,  1 / 4.0, 1 / 8.0,
                                        1 / 16.0, 1 / 8.0, 1 / 16.0  };
std::random_device int_rd;


int main(int argc, char *argv[]) {
    // Seed random number generator
    std::srand(time(0));
    // Populate a fraction of the cells
    std::vector<Agent *> agents = init_agents(int(WIDTH * HEIGHT * FRAC_CELLS));

    // Initialize GLUT
    glutInit(&argc, argv);                              
    // Set the window's display mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);        
    // Set the window's initial width & height
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);    
    // Position the window's initial top-left corner
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y); 
    // Continue program after window is closed
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    // Create a window with the given title
    glutCreateWindow("Slime Mold Simulation");          
    // Register display callback handler for window re-paint
    glutDisplayFunc(display);                           

    while (glutGetWindow()) {
        update(agents);
        glutPostRedisplay();
        glutMainLoopEvent();
        usleep(DELAY);
    }

    for (auto &agent : agents) {
        delete agent;
    }
}
