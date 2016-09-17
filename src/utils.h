#pragma once

#include <iostream>
#include <cassert>
#include <random>

#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>

#include "options.h"
#include "jsonConfig.h"


/**
 * Various functions implementation
 */
namespace utils {

/**
 * Generate a random integers between a and b, included.
 */
inline int64_t randnum(int64_t a, int64_t b) {
    std::random_device random_rd;
    std::mt19937 random_gen(random_rd());
    return std::uniform_int_distribution<int64_t>(a, b)(random_gen);
}


} // namespace utils
