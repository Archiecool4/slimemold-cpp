#ifndef __AGENT_HPP__
#define __AGENT_HPP__


/**
 * A class representing each agent on the grid.
 */
class Agent {
    private:
        // The x position of the agent
        double x_pos;
        // The y position of the agent
        double y_pos;
        // The orientation of the agent as an angle
        double orientation;

    public:
        /**
         * Places an agent on the grid with a random position and orientation.
         */
        Agent();
        
        /**
         * Moves an agent in accordance with its current position and orientation.
         */
        void move_slime();

        /**
         * Senses chemoattractants to compute a new orientation for the agent.
         */
        void sense_slime();
};


#endif // #ifndef __AGENT_HPP__