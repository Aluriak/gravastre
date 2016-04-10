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
// Orbital data
#define SYSTEM_JSON_KEY_SPEED "speed"
#define SYSTEM_JSON_KEY_DISTANCE "distance"
#define SYSTEM_JSON_KEY_APOAPSIS "apoapsis"
#define SYSTEM_JSON_KEY_PERIAPSIS "periapsis"
#define SYSTEM_JSON_KEY_ECCENTRICITY "eccentricity"
#define SYSTEM_JSON_KEY_SEMIMAJORAXIS "semimajoraxis"
#define SYSTEM_JSON_KEY_SEMIMINORAXIS "semiminoraxis"


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
