/*
 * File:   TaskEngine.cpp
 * Author: TheArni
 *
 * Created on 8 de abril de 2015, 16:02
 */

#include "TaskEngine.h"

#include <assert.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <SDL_thread.h>
#include <SDL_cpuinfo.h>
#include "Logger.h"

std::queue<Task*> tasks;
std::vector<SDL_Thread*> threads;

SDL_cond* taskAvailable;
SDL_mutex* threadLock;

int routine(void *data) {
    while (true) {
        SDL_LockMutex(threadLock);
        SDL_CondWait(taskAvailable, threadLock);
        Task* task = tasks.front();
        tasks.pop();
        task->run();
        delete task;
        SDL_UnlockMutex(threadLock);
    }
    return 0;
}

void TaskEngine::init() {
    taskAvailable = SDL_CreateCond();
    threadLock = SDL_CreateMutex();
    Logger::info << "Spawning " << SDL_GetCPUCount() << " threads" << std::endl;
    for (int i = 0; i < SDL_GetCPUCount(); i++) {
        threads.push_back(SDL_CreateThread(routine, "Worker Thread", NULL));
    }
}

void TaskEngine::addTask(Task *task) {
    tasks.push(task);
    SDL_CondSignal(taskAvailable);
}

void TaskEngine::waitThreads() {
    if (tasks.size() > 0) {
        SDL_LockMutex(threadLock);
        while (tasks.size() != 0) {
            SDL_CondWait(taskAvailable, threadLock);
        }
        SDL_UnlockMutex(threadLock);
    }
}

