/*
 * File:   LoadingState.h
 * Author: TheArni
 *
 * Created on 18 de abril de 2015, 19:36
 */

#ifndef LOADINGSTATE_H
#define	LOADINGSTATE_H

#include "State.h"

#include <string>

class LoadingState : public State {
public:

    LoadingState(std::string nextState) : stateName(nextState) {

    }

    virtual void onEnter();

    virtual void render();
    virtual void update();

private:
    std::string stateName;
};

#endif	/* LOADINGSTATE_H */

