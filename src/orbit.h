#pragma once


#include <iostream>
#include <tuple>
#include <random>

#include <QJsonArray>

#include "converters.h"
#include "utils.h"


namespace orbit {
/**
 * Return orbital speed of a body of mass m1 (kg) orbiting a body of mass m2 (kg)
 * at a distance r (meter).
 * The returned speed is in meter per second.
 * This is an approximation: don't allow elliptical orbits.
 */
inline double orbital_speed(double mass1, double mass2, double meter_dist) {
    return sqrt(PHYSIC_GRAVITATIONNAL_CONSTANT * (mass1 + mass2) / meter_dist);
}

/**
 * Return orbital speed of a body of mass m1 (kg) orbiting a body of mass m2 (kg)
 * at a distance r (meter), and with a semi major axis a.
 * The returned speed is in meter per second.
 * This allow elliptical orbits.
 */
inline double orbital_speed(double mass1, double mass2, double meter_dist, double semimajoraxis) {
    return sqrt(PHYSIC_GRAVITATIONNAL_CONSTANT * (mass1 + mass2) * (1. / meter_dist) - (2. / semimajoraxis));
}

/*
 * Return orbital distance of a body of mass m1 (kg) orbiting a body of mass m2 (kg)
 * at a speed v (m.s-1).
 * Returned distance is in meter.
 * This lead to circular orbit.
 */
inline double orbital_distance(double mass1, double mass2, double speed) {
    return PHYSIC_GRAVITATIONNAL_CONSTANT * (mass1 + mass2) / (speed * speed);
}


/**
 * Relations between orbital speed, semi major axis and distance.
 */
inline double speed_from_semimajoraxis_distance(double mass1, double mass2, double semimajoraxis, double distance) {
    return sqrt(PHYSIC_GRAVITATIONNAL_CONSTANT * (mass1 + mass2) * (2. / distance - 1. / semimajoraxis));
}
inline double semimajoraxis_from_speed_distance(double mass1, double mass2, double speed, double distance) {
    return -1. / (speed * speed / (PHYSIC_GRAVITATIONNAL_CONSTANT * (mass1 + mass2)) - 2. / distance);
}
inline double distance_from_speed_semimajoraxis(double mass1, double mass2, double speed, double semimajoraxis) {
    return 2. / (speed * speed / (PHYSIC_GRAVITATIONNAL_CONSTANT * (mass1 + mass2)) + 1. / semimajoraxis);
}

/**
 * Relations between apoapsis, periapsis and eccentricity.
 */
inline double eccentricity_from_apsis(double periapsis, double apoapsis) {
    return fabs((apoapsis - periapsis) / (apoapsis + periapsis));
}
inline double apoapsis_from_eccentricity(double eccentricity, double periapsis) {
    return (periapsis * (eccentricity + 1)) / (1. - eccentricity);
}
inline double periapsis_from_eccentricity(double eccentricity, double apoapsis) {
    return (apoapsis * (1. - eccentricity)) / (eccentricity + 1);
}

/**
 * Relations between semi major axis, apoapsis and periapsis.
 */
inline double semimajoraxis_from_periapsis_apoapsis(double periapsis, double apoapsis) {
    return (periapsis + apoapsis) / 2.;
}
inline double periapsis_from_semimajoraxis_apoapsis(double semimajoraxis, double apoapsis) {
    return 2. * semimajoraxis - apoapsis;
}
inline double apoapsis_from_semimajoraxis_periapsis(double semimajoraxis, double periapsis) {
    return 2. * semimajoraxis - periapsis;
}

/**
 * Relations between eccentricity, semi major axis, and semi minor axis.
 */
inline double eccentricity_from_semimajoraxis_semiminoraxis(double semimajoraxis, double semiminoraxis) {
    return sqrt(1. - (semiminoraxis * semiminoraxis) / (semimajoraxis * semimajoraxis));
}
inline double semimajoraxis_from_semiminoraxis_eccentricity(double semiminoraxis, double eccentricity) {
    return sqrt((semiminoraxis * semiminoraxis) / (1. - eccentricity * eccentricity));
}
inline double semiminoraxis_from_eccentricity_semimajoraxis(double eccentricity, double semimajoraxis) {
    return sqrt((semimajoraxis * semimajoraxis) - (semimajoraxis * semimajoraxis) * (eccentricity * eccentricity));
}


/**
 * Predicate, true if data available is enough to deduce an orbite between two objects.
 */
inline bool predictible(bool distance, bool periapsis, bool apoapsis, bool eccentricity,
                        bool speed, bool semimajoraxis, bool semiminoraxis,
                        bool mass_orbiter, bool mass_orbited) {
    return mass_orbited && mass_orbiter
        &&((distance)
        || (distance && speed)
        || (distance && periapsis)
        || (distance && periapsis && eccentricity)
        || (periapsis && apoapsis)
        || (eccentricity && periapsis)
        || (eccentricity && apoapsis)
        || (eccentricity && semimajoraxis)
        || (eccentricity && semiminoraxis));
}


inline double tangent_slope(double semiminoraxis, double semimajoraxis,
                            double x, double y) {
    return -(semiminoraxis * semiminoraxis * semiminoraxis * x)
             / (semimajoraxis * semimajoraxis * semimajoraxis * y);
}


/**
 * Value holder of orbital trajectory
 */
struct OrbitalTrajectory {
    double mass, parent_mass, speed, distance,
           speedX, speedY, positionX, positionY,
           apoapsis, periapsis, eccentricity,
           semimajoraxis, semiminoraxis;
    bool have_mass, have_parent_mass, have_speed, have_distance,
         have_speedX, have_speedY, have_positionX, have_positionY,
         have_apoapsis, have_periapsis, have_eccentricity,
         have_semimajoraxis, have_semiminoraxis;

    OrbitalTrajectory();
    OrbitalTrajectory(QJsonObject data, double* =NULL);


#if DEBUG_ORBITAL_TRAJECTORY_LOGS
    void debug();
#endif
    std::string haves() const;
    void compute();
    std::tuple<double, double, double, double> position() const;

    // debug
    double getWidth() const { return this->semimajoraxis * 1.; }
    double getHeight() const { return this->semiminoraxis * 1.; }


    bool is_orbit() const {
        // True if: all circular orbit data
        return (have_distance && have_speed && have_mass && have_parent_mass)
            // all non-circular orbit data
            || (have_eccentricity && have_semimajoraxis && have_semiminoraxis
                && have_mass && have_parent_mass);
    }
    bool is_precise_placement() const {
            return have_speedX && have_speedY && have_positionX && have_positionY;
    }
    bool is_valid() const {
        return is_orbit() or is_precise_placement();
    }
    bool is_circular_orbit() const {
        return (have_eccentricity and eccentricity == 0.) or (not have_eccentricity);
    }
    bool is_elliptic_orbit() const {
        return have_eccentricity and 0. < eccentricity and eccentricity < 1.;
    }
    bool enough_data() const {
        return orbit::predictible(have_distance, have_periapsis, have_apoapsis,
                                  have_eccentricity, have_speed, have_semimajoraxis,
                                  have_semiminoraxis, have_mass, have_parent_mass);
    }

}; // class OrbitalTrajectory




} // namespace orbit
