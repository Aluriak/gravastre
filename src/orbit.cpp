#include "orbit.h"


orbit::OrbitalTrajectory::OrbitalTrajectory() {
    have_mass = false;
    have_parent_mass = false;
    have_distance = false;
    have_speed = false;
    have_apoapsis = false;
    have_periapsis = false;
    have_eccentricity = false;
    have_semimajoraxis = false;
    have_semiminoraxis = false;
}


orbit::OrbitalTrajectory::OrbitalTrajectory(QJsonObject data, double* parent_mass_) {
    have_mass = data[SYSTEM_JSON_KEY_MASS].isDouble();
    have_parent_mass = parent_mass_ != NULL;
    have_distance = data[SYSTEM_JSON_KEY_DISTANCE].isDouble();
    have_speed = data[SYSTEM_JSON_KEY_SPEED].isDouble();
    have_apoapsis = data[SYSTEM_JSON_KEY_APOAPSIS].isDouble();
    have_periapsis = data[SYSTEM_JSON_KEY_PERIAPSIS].isDouble();
    have_eccentricity = data[SYSTEM_JSON_KEY_ECCENTRICITY].isDouble();
    have_semimajoraxis = data[SYSTEM_JSON_KEY_SEMIMAJORAXIS].isDouble();
    have_semiminoraxis = data[SYSTEM_JSON_KEY_SEMIMINORAXIS].isDouble();
    mass = data[SYSTEM_JSON_KEY_MASS].toDouble();
    parent_mass = parent_mass_ != NULL? *parent_mass_ : 0.;
    distance = unit::au_to_meter(data[SYSTEM_JSON_KEY_DISTANCE].toDouble());
    speed = unit::au_to_meter(data[SYSTEM_JSON_KEY_SPEED].toDouble());
    apoapsis = unit::au_to_meter(data[SYSTEM_JSON_KEY_APOAPSIS].toDouble());
    periapsis = unit::au_to_meter(data[SYSTEM_JSON_KEY_PERIAPSIS].toDouble());
    eccentricity = data[SYSTEM_JSON_KEY_ECCENTRICITY].toDouble();
    semimajoraxis = unit::au_to_meter(data[SYSTEM_JSON_KEY_SEMIMAJORAXIS].toDouble());
    semiminoraxis = unit::au_to_meter(data[SYSTEM_JSON_KEY_SEMIMINORAXIS].toDouble());
    this->compute(); // find values that can be
}


#if DEBUG_ORBITAL_TRAJECTORY_LOGS
void orbit::OrbitalTrajectory::debug() {
    std::cerr << "TRAJECTORY " << this <<  " :\n"
        << "\tMass: " << (have_mass? std::to_string(mass) : "unknow") << std::endl
        << "\tParent mass: " << (have_parent_mass? std::to_string(parent_mass) : "unknow") << std::endl
        << "\tDistance: " << (have_distance? std::to_string(distance) : "unknow") << std::endl
        << "\tSpeed: " << (have_speed? std::to_string(speed) : "unknow") << std::endl
        << "\tApoapsis: " << (have_apoapsis? std::to_string(apoapsis) : "unknow") << std::endl
        << "\tPeriapsis: " << (have_periapsis? std::to_string(periapsis) : "unknow") << std::endl
        << "\tEccentricity: " << (have_eccentricity? std::to_string(eccentricity) : "unknow") << std::endl
        << "\tSemi major axis: " << (have_semimajoraxis? std::to_string(semimajoraxis) : "unknow") << std::endl
        << "\tSemi minor axis: " << (have_semiminoraxis? std::to_string(semiminoraxis) : "unknow") << std::endl;
}
#endif


std::string orbit::OrbitalTrajectory::haves() const {
    std::string out(std::to_string(have_mass) + std::to_string(have_parent_mass)
                  + std::to_string(have_speed) + std::to_string(have_distance)
                  + std::to_string(have_apoapsis) + std::to_string(have_periapsis)
                  + std::to_string(have_eccentricity)
                  + std::to_string(have_semimajoraxis));
    return out;
}

/**
 * Seek to complete the instance values, using equations.
 */
void orbit::OrbitalTrajectory::compute() {
    // update the found data, until distance and speed is found
    bool change = true;
    while(change) {
        if(have_apoapsis && have_eccentricity && have_parent_mass && not have_periapsis) {
            have_periapsis = true;
            periapsis = orbit::periapsis_from_eccentricity(eccentricity, apoapsis);
        } else if(have_periapsis && have_eccentricity && have_parent_mass && not have_apoapsis) {
            have_apoapsis = true;
            apoapsis = orbit::apoapsis_from_eccentricity(eccentricity, periapsis);
        } else if(have_periapsis && have_apoapsis && not have_eccentricity) {
            have_eccentricity = true;
            eccentricity = orbit::eccentricity_from_apsis(periapsis, apoapsis);
        } else if(have_semimajoraxis && have_distance && have_mass && have_parent_mass && not have_speed) {
            have_speed = true;
            speed = speed_from_semimajoraxis_distance(mass, parent_mass, semimajoraxis, distance);
        } else if(have_apoapsis && have_periapsis && not have_semimajoraxis) {
            have_semimajoraxis = true;
            semimajoraxis = semimajoraxis_from_periapsis_apoapsis(periapsis, apoapsis);
        } else if(have_semimajoraxis && have_apoapsis && not have_periapsis) {
            have_periapsis = true;
            periapsis = periapsis_from_semimajoraxis_apoapsis(semimajoraxis, apoapsis);
        } else if(have_semimajoraxis && have_periapsis && not have_apoapsis) {
            have_apoapsis = true;
            apoapsis = apoapsis_from_semimajoraxis_periapsis(semimajoraxis, periapsis);
        } else if(have_speed && have_distance && have_mass && have_parent_mass && not have_semimajoraxis) {
            have_semimajoraxis = true;
            semimajoraxis = semimajoraxis_from_speed_distance(mass, parent_mass, speed, distance);
        } else if(have_speed && have_semimajoraxis && have_mass && have_parent_mass && not have_distance) {
            have_distance = true;
            distance = distance_from_speed_semimajoraxis(mass, parent_mass, speed, semimajoraxis);
        } else if(have_distance && have_semimajoraxis && have_mass && have_parent_mass && not have_speed) {
            have_speed = true;
            speed = orbit::orbital_speed(mass, parent_mass, distance, semimajoraxis);
        } else if(have_distance && have_mass && have_parent_mass && not have_speed) {
            have_speed = true;
            speed = orbit::orbital_speed(mass, parent_mass, distance);
        } else if(have_speed && have_mass && have_parent_mass && not have_distance) {
            have_distance = true;
            distance = orbit::orbital_distance(mass, parent_mass, speed);
        } else if(have_semiminoraxis && have_semimajoraxis && not have_eccentricity) {
            have_eccentricity = true;
            eccentricity = eccentricity_from_semimajoraxis_semiminoraxis(semimajoraxis, semiminoraxis);
        } else if(have_eccentricity && have_semiminoraxis && not have_semimajoraxis) {
            have_semimajoraxis = true;
            semimajoraxis = semimajoraxis_from_semiminoraxis_eccentricity(semiminoraxis, eccentricity);
        } else if(have_semimajoraxis && have_eccentricity && not have_semiminoraxis) {
            have_semiminoraxis = true;
            semiminoraxis = semiminoraxis_from_eccentricity_semimajoraxis(eccentricity, semimajoraxis);
        } else {
            // nothing to do
            change = false;
        }
    } // end while(change)
} // end OrbitalTrajectory::compute()


/**
 * Return coordinates and speed of orbiting body.
 * Returned value can change between two calls.
 */
std::tuple<double, double, double, double> orbit::OrbitalTrajectory::position() const {
    double pos_x = 0., pos_y = 0., speed_x = 0., speed_y = 0.;
    if(this->is_orbit()) {
        // Random value generator
        std::random_device random_rd;     // only used once to initialise (seed) engine
        std::mt19937 random_gen(random_rd());    // random-number engine used (Mersenne-Twister in this case)
        // Compute a random position
        pos_x = std::uniform_real_distribution<double>(-semimajoraxis, semimajoraxis)(random_gen);
        // equation of an ellipse: y = sqrt(b*b - b*b * x*x / (a*a))
        pos_y = sqrt(semiminoraxis*semiminoraxis - semiminoraxis*semiminoraxis * pos_x*pos_x / (semimajoraxis * semimajoraxis));
        // Compute the speed associated with position, in this particular case
        double real_distance = sqrt(pos_x*pos_x + pos_y*pos_y);
        double real_speed = orbit::orbital_speed(mass, parent_mass,
                                                 real_distance, semimajoraxis);
        // Now, determine the speed on x axis and on y axis.
        // Equation of the tangent to an ellipse:
        // 1 = x*α / (a*a) + y*(b/a)β / (b*b)
        // With (α;(b/a)β) the vertex on the ellipse, a the semi-major axis,
        //      b the semi-minor axis and (x;y) the vertex on the tangent.
        // Here is the tangent equation:
        // y = -(b*α / (β*a*a))*x + b/β
        // The slope is then -(b*α / (β*a*a)).
        // The slope equals to tan(þ), with þ the angle given by the line and the abscissa.
        // By construction, speed is the hypothenuse of the triangle formed by
        //     - the position of astre (x, y),
        //     - the vertex at a distance 'speed' (x', y') on the ellipse tangent,
        //     - the vertex at (x', y) or (x, y').
        // Then, the side [x', x] gives the speed on x axis,
        //  and the side [y, y'] gives the speed on y axis.
        // These two values are computable as sin(þ) = [y, y'] / hypothenuse
        //  and cos(þ) = [x, x'] / hypothenuse.
        // Here are the final equations:
        //     speed_x = speed * cos(atan(slope))
        //     speed_y = speed * sin(atan(slope))
        // Nota bene: cos(atan(x)) <=> 1 / sqrt(x*x + 1)
        //            sin(atan(x)) <=> x / sqrt(x*x + 1)
        double slope(tangent_slope(semiminoraxis, semimajoraxis, pos_x, pos_y));
        speed_x =     1 * real_speed / sqrt(slope * slope + 1);
        speed_y = slope * real_speed / sqrt(slope * slope + 1);

        // Above or below the parent ?  (considering y axis)
        if(std::bernoulli_distribution()(random_gen)) {
            // without position swap, astre could'nt be placed above the parent
            pos_y *= -1;
            // without speed swap, astre will orbit clockwise if spawned below the parent
            speed_x *= -1;
        }
    }
    return std::make_tuple(pos_x, pos_y, speed_x, speed_y);
}
