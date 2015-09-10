/*
 * File:   TaskEngine.h
 * Author: TheArni
 *
 * Created on 8 de abril de 2015, 16:02
 */

#ifndef TASK_ENGINE_H
#define	TASK_ENGINE_H

#include <functional>

/**
 * This class is used by the Task Engine. It is simply a wrapper for a function
 * to be executed in a threaded environment.
 */
class Task {
public:

    /**
     * This function contains the code that will be executed once the Task
     * Engine has processed this object.
     */
    virtual void run() = 0;
};

/**
 * This class is used as a wrapper to convert a lambda function to a Task
 * suitable for the Task Engine.
 * 
 * Please note that this class is used for convinience, and if there's going to
 * be heavy ammounts of instancing, a propper object extending the Task class
 * should be used.
 */
class LambdaTask : public Task {
public:

    LambdaTask(std::function<void() > lambda) {
        this->lambda = lambda;
    }

    virtual void run() {
        lambda();
    }
private:
    std::function<void() > lambda;
};

namespace TaskEngine {
    
    /**
     * Initializes the Task Engine, spawning as many threads as logical cores
     * available for maximum multitasking performance.
     * 
     * <b>Note</b>: this is automatically called when initializing the Game. 
     */
    void init();

    /**
     * Adds a task to the worker thread cues.
     * @param task The task to be run in the threaded environment.
     */
    void addTask(Task *task);

    /**
     * Waits and blocks the current thread until the worker thread's tasks cue
     * is empty.
     */
    void waitThreads();
};

#endif	/* TASK_ENGINE_H */

