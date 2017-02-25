#pragma once

#include <iostream>

#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>

#include "options.h"


/**
 * JSON related constants
 */
#define SYSTEM_JSON_KEY_NAME "name"
#define SYSTEM_JSON_KEY_MASS "mass"
#define SYSTEM_JSON_KEY_COLOR "color"
#define SYSTEM_JSON_KEY_RADIUS "radius"  // km
#define SYSTEM_JSON_KEY_SATELLITES "satellites"
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


/**
 * Various functions implementation
 */
namespace utils {

inline QJsonArray json_from_file(std::string filename) {
      QFile file(filename.c_str());
      if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          std::cerr << "File " << filename << " not opened." << std::endl;
          exit(1);
      }
      return QJsonDocument::fromJson(file.readAll()).array();
}


/**
 * Generate a random integers between a and b, included.
 */
inline int64_t randnum(int64_t a, int64_t b) {
    std::random_device random_rd;
    std::mt19937 random_gen(random_rd());
    return std::uniform_int_distribution<int64_t>(a, b)(random_gen);
}


} // namespace utils
