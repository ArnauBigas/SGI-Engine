#include "Profiler.h"
#include "Timer.h"
#include <iostream>

Profiler::Profile *base;
Profiler::Profile *cProf = NULL;

Profiler::Profile *Profiler::Profile::get(std::string name) {
    for (Profile *p : children) {
        if (p->name == name) {
            return p;
        }
    }
    return NULL;
}

void Profiler::Profile::reset() {
    for (Profile *p : children) {
        p->reset();
    }
    timer.reset();
}

Profiler::Profile::~Profile() {
    for (Profile *p : children) {
        delete p;
    }
    std::cout << "Profile destructed" << std::endl;
}

void Profiler::init() {
    base = new Profile();
    base->name = "base";
    cProf = base;
}

void Profiler::cleanup() {
    while (cProf->parent != NULL) {
        cProf = cProf->parent;
    }
    delete cProf;
}

void Profiler::start(std::string name) {
    Profile *profile;
    bool exists = false;

    for (Profile *p : cProf->children) {
        if (p->name == name) {
            exists = true;
            profile = p;
            break;
        }
    }

    if (!exists) {
        profile = new Profile();
        profile->name = name;
        profile->parent = cProf;
        profile->timer.reset();
        cProf->children.push_back(profile);
    }

    profile->timer.start();

    cProf = profile;
}

void Profiler::stop() {
    cProf->timer.stop();
    cProf = cProf->parent;
}

void Profiler::reset() {
    cProf = base;
    cProf->reset();
}

Profiler::Profile *Profiler::get(std::string name) {
    return base->get(name);
}