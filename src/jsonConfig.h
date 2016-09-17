#pragma once

#include <iostream>
#include <cassert>

#include <QFile>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>

#include "utils.h"
#include "converters.h"



namespace utils {

    /**
     * Small data holder for player
     */
    struct Player {
        Player(std::string name) : name(name) {};
        std::string name;
    };


    /**
     * Wrapper around JSON file reading and access to expected fields.
     */
    class JsonConfig {

    public:
        // Lifetime
        JsonConfig();
        JsonConfig(const JsonConfig&); // copy
        JsonConfig(std::string);
        JsonConfig(QJsonDocument);
        ~JsonConfig();

        // Methods

        // Accessors
        QJsonObject getOptions() const { return this->options; };
        QJsonArray  getSystems() const { return this->systems; };
        std::vector<Player*> getPlayers() const { return this->players; };
        Player* getPlayer(uint64_t idx) const { return this->players[idx]; };


    private:
        // Methods
        void parseDocument();
        void validateDocument();

        // Attributes
        QJsonDocument doc;
        QJsonObject options;
        std::vector<Player*> players;
        QJsonArray systems;
    };

} // end namespace utils




/**
 * JSON related constants
 */
// JSON file main roots
#define SYSTEM_JSON_ROOT_OPTIONS "Options"
#define SYSTEM_JSON_ROOT_PLAYERS "Players"
#define SYSTEM_JSON_ROOT_SYSTEMS "Systems"

// System data
#define SYSTEM_JSON_KEY_NAME "name"
#define SYSTEM_JSON_KEY_MASS "mass"
#define SYSTEM_JSON_KEY_COLOR "color"
#define SYSTEM_JSON_KEY_RADIUS "radius"
#define SYSTEM_JSON_KEY_SATELLITES "satellites"
#define SYSTEM_JSON_KEY_DIAMETER "diameter"

// Orbital data (and expected units)
#define SYSTEM_JSON_KEY_SPEED "speed"  // m.s-1
#define SYSTEM_JSON_KEY_DISTANCE "distance"  // AU
#define SYSTEM_JSON_KEY_APOAPSIS "apoapsis"  // AU
#define SYSTEM_JSON_KEY_PERIAPSIS "periapsis"  // AU
#define SYSTEM_JSON_KEY_ECCENTRICITY "eccentricity"  // no unit, 0 < e <= 1
#define SYSTEM_JSON_KEY_SEMIMAJORAXIS "semimajoraxis"  // AU
#define SYSTEM_JSON_KEY_SEMIMINORAXIS "semiminoraxis"  // AU
#define SYSTEM_JSON_KEY_SPEEDX "speedX"  // m.s-1
#define SYSTEM_JSON_KEY_SPEEDY "speedY"  // m.s-1
#define SYSTEM_JSON_KEY_POSITIONX "positionX"  // AU
#define SYSTEM_JSON_KEY_POSITIONY "positionY"  // AU

// Player data
#define SYSTEM_JSON_KEY_ISPLAYER "is player"  // bool
#define SYSTEM_JSON_KEY_KEY_THRUST "key:thrust"  // string
