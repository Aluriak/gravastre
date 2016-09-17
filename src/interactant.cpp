#include "interactant.h"


eng::Interactant::Interactant(double mass, double pos_x, double pos_y,
                              double speed_x, double speed_y, std::string name) :
    QGraphicsItem(NULL)
{
    this->init_attributes(mass, pos_x, pos_y, speed_x, speed_y, name);
}


eng::Interactant::~Interactant() {
#if DATA_INTERACTANT_HOLDS_TRAJECTORY
    if(this->trajectory != NULL) { delete this->trajectory; }
#endif
}


void eng::Interactant::physic_update() {
    // add acceleration to speed and speed to position
    this->speed_x += this->accel_x;
    this->speed_y += this->accel_y;
    this->position_x += unit::real_speed_to_simulation_speed(unit::meter_to_au(speed_x));
    this->position_y += unit::real_speed_to_simulation_speed(unit::meter_to_au(speed_y));
    // prepare the next step
    this->last_accel_x = this->accel_x;
    this->last_accel_y = this->accel_y;
    this->accel_x = this->getBasicAccelX();
    this->accel_y = this->getBasicAccelY();
    // Graphic
    this->setGraphicalPosition();
}


/**
 * Compute the acceleration to given Interactant, and add it to the acceleration accumulator.
 * if distance is not valid (<0), it will be computed.
 */
void eng::Interactant::accelerateTo(eng::Interactant* const othr, double dist) {
    if(dist < 0.) dist = distTo(othr);
    assert(othr != NULL);
    //double attraction = - GRAVITATIONNAL_CONSTANT * othr->mass * this->mass / squared_dist;
    // get graviationnal force F (in newton)
    double attraction = unit::attraction_force_meter(
            this->mass, othr->mass, unit::au_to_meter(dist));
    // add it to acceleration counters
    // F=ma <=> acceleration = F/mass
    this->accel_x += attraction * ((this->position_x - othr->position_x) / dist) / this->mass;
    this->accel_y += attraction * ((this->position_y - othr->position_y) / dist) / this->mass;

#if DEBUG_SUN_DIST
    if(othr->dist_to_sun_min < 0.) { // DEBUG
        othr->dist_to_sun_min = dist;
        othr->dist_to_sun_max = dist;
    }

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


void eng::Interactant::absorbs(eng::Interactant* const othr) {
    double mass_ratio = othr->mass / (othr->mass + this->mass);
#if DEBUG_ABSORBED_LOGS
    std::cout << this->name << " absorbs " << othr->name << std::endl;
    std::cout << "\tmass ratio: " << mass_ratio << std::endl;
#endif
    this->mass += othr->mass;
    this->speed_x = (1.-mass_ratio) * this->speed_x + (mass_ratio * othr->speed_x);
    this->speed_y = (1.-mass_ratio) * this->speed_y + (mass_ratio * othr->speed_y);
    this->accel_x = this->accel_x + othr->accel_x;
    this->accel_y = this->accel_y + othr->accel_y;
    othr->nullify();  // othr is now unconsiderable by all astres
}


void eng::Interactant::nullify() {
#if DEBUG_NULLIFIED_LOGS
    std::cout << this->name << " nullified !" << std::endl;
#endif
    this->nullified = true;
    this->visible = false;
}


/**
 * Return a distance in AU
 */
double eng::Interactant::distTo(const Interactant* const othr) const {
    // while the position_x and position_y attributes are in AU,
    //   no conversion is necessary.
    double dist = sqrt(
        (this->position_x - othr->position_x) * (this->position_x - othr->position_x)
      + (this->position_y - othr->position_y) * (this->position_y - othr->position_y));
#if DEBUG_DISTANCES_LOGS
    std::cout << this->name << " (" << this->position_x << ";" << this->position_y
              << ") is distant from "
              << othr->name << " (" << othr->position_x << ";" << othr->position_y
              << ") of " << dist << " au" << std::endl;
#endif
    return dist;
}


/**
 * Predicate: is there a collision with this other Interactant ?
 * Implemented simply by comparing the position. Interactant subclasses should redefine
 * this method if they don't want to be a dot in the space.
 */
bool eng::Interactant::collide(const Interactant* const othr) const {
#if DEBUG_COLLISION_LOGS
    if(collidesWithItem(othr)) {
        std::cout << "Collision dist: " << distTo(othr) <<  std::endl;
        return true;
    } else {
        return false;
    }
#else
    return QGraphicsItem::collidesWithItem(othr);
#endif
}




/**
 * PRIVATE: change position of graphical representation
 */
void eng::Interactant::setGraphicalPosition() {
    this->setPos(unit::au_to_pixel(this->position_x), unit::au_to_pixel(this->position_y));
}




/**
 * PRIVATE: Initialize fields. Called by constructors.
 */
void eng::Interactant::init_attributes(double mass, double pos_x, double pos_y,
                                       double speed_x, double speed_y, std::string name) {
    this->name = name;
    this->mass = mass;
    this->position_x = pos_x;
    this->position_y = pos_y;
    this->setGraphicalPosition();
#if VIEW_INITIAL_POSITION
    this->init_position_x = pos_x;
    this->init_position_y = pos_y;
#endif
    this->speed_x = speed_x;
    this->speed_y = speed_y;
    this->accel_x = 0.;
    this->accel_y = 0.;
    this->visible = true;
    this->nullified = false;
    assert(mass != 0.);
#if DEBUG_SUN_DIST
    // debug
    dist_to_sun_min = -1;
    dist_to_sun_max = -1;
#endif
}
