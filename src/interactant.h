#pragma once


#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <QGraphicsItem>

#include "converters.h"
#include "orbit.h"




namespace eng {



    /**
     * Object evolving in 2D space, interacting gravitationnaly with the others.
     */
    class Interactant : public QGraphicsItem {

    public:
        // Lifetime
        Interactant(double, double, double, double, double, std::string);
        ~Interactant();

        // Methods
        virtual void physic_update();
        virtual void graphic_update() { QGraphicsItem::update(); };
        void accelerateTo(Interactant* const, double dist=-1);
        virtual void absorbs(Interactant* const);
        void nullify();
        double distTo(const Interactant* const) const;  // distance in UA

        // Accessors
        std::string getName() const { return this->name; }
        double getX() const { return this->position_x; }
        double getY() const { return this->position_y; }
        double getSpeedX() const { return this->speed_x; }
        double getSpeedY() const { return this->speed_y; }
        double getAccelX() const { return this->last_accel_x; }
        double getAccelY() const { return this->last_accel_y; }
        double getMass() const { return this->mass; }
        virtual double getBasicAccelX() const { return 0; }
        virtual double getBasicAccelY() const { return 0; }

        // Predicates
        bool collide(const Interactant* const) const;
        bool bigger(const Interactant* const othr) const { return this->mass > othr->mass; }
        bool updatable() const { return not this->nullified; }


#if VIEW_INITIAL_POSITION
        double getInitX() const { return this->init_position_x; }
        double getInitY() const { return this->init_position_y; }
#endif


#if DATA_INTERACTANTS_HOLDS_TRAJECTORY
        orbit::OrbitalTrajectory& getTrajectory() const { return *this->trajectory; }
        void setTrajectory(orbit::OrbitalTrajectory t)
                                    { this->trajectory = new orbit::OrbitalTrajectory(t); }
        bool haveTrajectory() const { return this->trajectory != NULL; }
#endif


    private:
        // Methods
        void init_attributes(double, double, double, double, double, std::string);
        void setGraphicalPosition();

        // Attributes
        std::string name;
        double mass; // mass in kg
        double position_x, position_y; // position in astronomic unit (AU)
        double speed_x, speed_y; // speed in meter per second (m.s-1)
        double accel_x, accel_y; // m.s-2
        double last_accel_x, last_accel_y;  // previous value of acceleration
        bool visible;  // false if should be hide
        bool nullified;  // true if should be free'd

#if DEBUG_SUN_DIST
        double dist_to_sun_min, dist_to_sun_max;
#endif
#if DATA_ASTRE_HOLDS_TRAJECTORY
        orbit::OrbitalTrajectory* trajectory = NULL;
#endif
#if VIEW_INITIAL_POSITION
        double init_position_x, init_position_y;
#endif

    };

} // namespace eng
