#pragma once

#include <iostream>

#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>


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
