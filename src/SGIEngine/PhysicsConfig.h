/*
 * File:   Physics_Config.h
 * Author: TheArni
 *
 * Created on 4 de marzo de 2015, 14:55
 */

#ifndef PHYSICS_CONFIG_H
#define	PHYSICS_CONFIG_H

struct PhysicsConfig {
    double scale = 1;
    double gravity = 9.81;
    double correctionThreshold = 0.01;
    double correctionPercentage = 0.2;
};

#endif	/* PHYSICS_CONFIG_H */

