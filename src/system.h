#pragma once

#include <iostream>
#include <assert.h>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "utils.h"
#include "astre.h"


/**
 * JSON related constants
 */
#define SYSTEM_JSON_KEY_NAME "name"
#define SYSTEM_JSON_KEY_MASS "mass"
#define SYSTEM_JSON_KEY_SPEED "speed"
#define SYSTEM_JSON_KEY_COLOR "color"
#define SYSTEM_JSON_KEY_SATELLITES "satellites"
#define SYSTEM_JSON_KEY_DISTANCE "distance"




namespace eng {

    /**
     * Factory of Astre, and a Composite.
     */
    class System {

    public:
        System(Astre*, std::vector<System*>, double);
        static std::vector<eng::System*> from_json(std::string);
        static std::vector<eng::System*> from_json(QJsonArray);
        static Astre* json_to_astre(QJsonObject);
        static void validate(QJsonObject system);


        // Accessors
        Astre* getAstre() const { return this->astre; }
        std::vector<System*> getSubsystems() const { return this->subsystems; }
        double getDistance() const { return this->distance; }


    private:
        Astre* astre;
        std::vector<System*> subsystems;
        double distance;

    };

} // namespace view