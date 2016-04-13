#pragma once

#include <iostream>

#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>



/**
 * JSON related constants
 */
#define SYSTEM_JSON_KEY_NAME "name"
#define SYSTEM_JSON_KEY_MASS "mass"
#define SYSTEM_JSON_KEY_COLOR "color"
#define SYSTEM_JSON_KEY_RADIUS "radius"
#define SYSTEM_JSON_KEY_SATELLITES "satellites"
// Orbital data (and expected units)
#define SYSTEM_JSON_KEY_SPEED "speed"  // m.s-1
#define SYSTEM_JSON_KEY_DISTANCE "distance"  // AU
#define SYSTEM_JSON_KEY_APOAPSIS "apoapsis"  // AU
#define SYSTEM_JSON_KEY_PERIAPSIS "periapsis"  // AU
#define SYSTEM_JSON_KEY_ECCENTRICITY "eccentricity"  // no unit, 0 < e <= 1
#define SYSTEM_JSON_KEY_SEMIMAJORAXIS "semimajoraxis"  // AU
#define SYSTEM_JSON_KEY_SEMIMINORAXIS "semiminoraxis"  // AU


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


} // namespace utils
