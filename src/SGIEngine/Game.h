/*
 * File:   Game.h
 * Author: TheArni
 *
 * Created on 15 de febrero de 2015, 13:48
 */

#ifndef GAME_H
#define	GAME_H

#include <string>

#include "State.h"

namespace Game {

    struct PerformanceData {
        long long frames;
        long long ticks;
        int fps;
        int tps;
    };

    /**
     * Initializes the game along with all the other engines
     * 
     * Loads the configuration file or creates it with the default values.
     * 
     * @param title The string that will be displayed at the top of the window
     * 
     * @return true if the game has been initialized successfully
     */
    bool init(std::string title, bool client);

    /**
     * Starts the main game loop, will only return after the game is closed.
     * 
     * You can stop the game by calling Game::stop().
     * 
     * Before calling this method, make sure you have added and entered a state.
     */
    void start();

    /**
     * Enters a new state and notifies both the previous and the next state
     * the change.
     * 
     * @param name The name of the new state, the one specified when calling
     * Game::addState()
     * 
     * @return true if the game could switch states, false if it couldn't (and 
     * it's still running the previous state).
     */
    bool enterState(std::string name);

    /**
     * Adds a new state to the game so that it can be entered using 
     * Game::enterState()
     * 
     * @param name The name of the state, used when switching states using
     * Game::enterState()
     * 
     * @param state The State object itself.
     */
    void addState(std::string name, State* state);

    /**
     * Returns the state object specified by the name.
     * 
     * @param name The name of the state to get.
     * 
     * @return The State named by the name variable.
     */
    State* getState(std::string name);

    /**
     * Safely stops the game (on the next tick) and stops all the other engines.
     */
    void stop();

    /**
     * Returns the amount of time the last tick took.
     * @return The duration in microseconds.
     */
    long lastTickTime();

    /**
     * Returns whether the game is a client instance or a server instance.
     * @return ditto.
     */
    bool isClient();

    /**
     * Kills the game, along with all the other engines and frees all resources.
     * Warning: it is recommended the use of the stop function instead, this
     * function should only be used for emergencies.
     */
    void kill();
    
    void setPerformaceDataFunc(void (*func)(PerformanceData));
};

#endif	/* GAME_H */

