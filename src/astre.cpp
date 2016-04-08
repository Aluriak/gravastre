#include "astre.h"


eng::Astre::Astre(double mass, double radius, double pos_x, double pos_y,
                  double speed_x, double speed_y, std::string name, QColor color) {
    this->mass = mass;
    this->radius = radius;
    this->position_x = pos_x;
    this->position_y = pos_y;
    this->speed_x = speed_x;
    this->speed_y = speed_y;
    this->accel_x = 0.;
    this->accel_y = 0.;
    this->name = name;
    this->color = color;
    this->visible = true;
    this->nullified = false;
    assert(mass != 0.);
#if DEBUG_SUN_DIST
    // debug
    dist_to_sun_min = -1;
    dist_to_sun_max = -1;
#endif
}



void eng::Astre::update() {
    // add acceleration to speed and speed to position
    // F=ma <=> acceleration = F/mass
    this->speed_x += this->accel_x / this->mass;
    this->speed_y += this->accel_y / this->mass;
    this->position_x += unit::real_speed_to_simulation_speed(unit::meter_to_au(speed_x));
    this->position_y += unit::real_speed_to_simulation_speed(unit::meter_to_au(speed_y));
    // prepare the next step
    this->accel_x = 0;
    this->accel_y = 0;
}


void eng::Astre::accelerateTo(eng::Astre* const othr, const double dist) {
    assert(othr != NULL);
    //double attraction = - GRAVITATIONNAL_CONSTANT * othr->mass * this->mass / squared_dist;
    // get graviationnal force F (in newton)
    double attraction = unit::attraction_force_meter(
            this->mass, othr->mass, unit::au_to_meter(dist));
    // add it to acceleration counters
    this->accel_x += attraction * ((this->position_x - othr->position_x) / dist);
    this->accel_y += attraction * ((this->position_y - othr->position_y) / dist);

#if DEBUG_SUN_DIST
    if(othr->dist_to_sun_min < 0.) { // DEBUG
        othr->dist_to_sun_min = dist;
        othr->dist_to_sun_max = dist;
    }

    // DEBUG testing  (NB: these lines modify the other astre)
    if(this->name == "sun") {
        if(dist < othr->dist_to_sun_min) othr->dist_to_sun_min = dist;
        if(dist > othr->dist_to_sun_max) othr->dist_to_sun_max = dist;
        std::cout << "DIST " << othr->name
            << "-sun: min=" << othr->dist_to_sun_min
            << "; max=" << othr->dist_to_sun_max
            << "; diff=" << othr->dist_to_sun_max - othr->dist_to_sun_min << std::endl;
    }
#endif
}


void eng::Astre::absorbs(eng::Astre* const othr) {
    double mass_ratio = othr->mass / (othr->mass + this->mass);
#if DEBUG_ABSORBED_LOGS
    std::cout << this->name << " absorbs " << othr->name << std::endl;
    std::cout << "\tmass ratio: " << mass_ratio << std::endl;
#endif
    this->mass += othr->mass;
    this->radius = Astre::mass_to_radius(this->mass);
    this->speed_x = (1.-mass_ratio) * this->speed_x + (mass_ratio * othr->speed_x);
    this->speed_y = (1.-mass_ratio) * this->speed_y + (mass_ratio * othr->speed_y);
    this->accel_x = this->accel_x + othr->accel_x;
    this->accel_y = this->accel_y + othr->accel_y;
    othr->nullify();  // othr is now unconsiderable by all astres
}


void eng::Astre::nullify() {
#if DEBUG_NULLIFIED_LOGS
    std::cout << this->name << " nullified !" << std::endl;
#endif
    this->nullified = true;
    this->visible = false;
}


/**
 * Return a distance in AU
 */
double eng::Astre::distTo(const Astre* const othr) const {
    // while the position_x and position_y attributes are in AU,
    //   no conversion is necessary.
    double dist = sqrt(
        (this->position_x - othr->position_x) * (this->position_x - othr->position_x)
      + (this->position_y - othr->position_y) * (this->position_y - othr->position_y));
#if DEBUG_DISTANCES_LOGS
    std::cout << this->name << " (" << this->position_x << ";" << this->position_y
              << ") is distant from "
              << othr->name << " (" << othr->position_x << ";" << othr->position_y
              << ") to " << dist << " au" << std::endl;
#endif
    return dist;
}

bool eng::Astre::collide(const Astre* const othr, const double dist) const {
#if COLLISION
    double min_separation = this->radius + othr->radius;
#if DEBUG_COLLISION_LOGS
    std::cout << "Collision: " << min_separation << "\tdist: " << dist <<  std::endl;
#endif
    return unit::au_to_pixel(dist) <= min_separation;
#else
    return false;
#endif
}
