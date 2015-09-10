/*
 * File:   Config.h
 * Author: TheArni
 *
 * Created on 16 de febrero de 2015, 22:32
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <document.h>

#include "GraphicsConfig.h"
#include "PhysicsConfig.h"

namespace Config {

    /**
     * Writes all the current configuration data to the Document, for later
     * saving into a file.
     * 
     * This method is usually used internally by the game engine itself.
     * 
     * @param doc The document to which the current configuration should be
     * saved.
     */
    void serialize(rapidjson::Document* doc);

    /**
     * Reads all the information in the document and sets the variables in the
     * configuration structs.
     * 
     * This method is usually used internally by the game engine itself.
     * 
     * @param doc The document that contains the configuration data to be loaded
     */
    void deserialize(rapidjson::Document* doc);

    /**
     * The struct containing graphical configurations.
     */
    extern GraphicsConfig graphics;
    
    /**
     * The struct containing physics configurations.
     */
    extern PhysicsConfig physics;
};

#endif	/* CONFIG_H */

