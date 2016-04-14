#pragma once


#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <QColor>
#include <QPainter>
#include <QGraphicsEllipseItem>

#include "converters.h"
#include "orbit.h"




namespace eng {

    /**
     * Astre data as tuple of four values:
     *  mass, radius, name and color.
     * Other data are position and speed and are defined elsewhere.
     * Note that its just a convenient wrapper: Astre class don't use it internally.
     */
    typedef std::tuple<double, double, std::string, QColor> AstreData;

    /**
     * 4-uplet defining all useful information about a object in a 2D world:
     *  position in x, postition in y, speed in x, speed in y
     * Note that its just a convenient wrapper: Astre class don't use it internally.
     */
    typedef std::tuple<double, double, double, double> PositionAndSpeed;


    /**
     * Object evolving in 2D space, interacting gravitationnaly with the others.
     */
    class Astre : public QGraphicsEllipseItem {

    public:
        Astre(double, double, double, double, double, double, std::string, QColor);
        Astre(double, double, double, double, double, std::string, QColor);
        ~Astre();
        void update();
        void accelerateTo(Astre* const, const double);
        void absorbs(Astre* const);
        void nullify();
        double distTo(const Astre* const) const;

        // Accessors
        std::string getName() const { return this->name; }
        double getX() const { return this->position_x; }
        double getY() const { return this->position_y; }
        double getSpeedX() const { return this->speed_x; }
        double getSpeedY() const { return this->speed_y; }
        double getMass() const { return this->mass; }
        double getRadius() const { return this->radius; }
        QColor getColor() const { return this->color; }

        bool collide(const Astre* const, const double) const;
        bool bigger(const Astre* const othr) const { return this->mass > othr->mass; }
        bool updatable() const { return not this->nullified; }

        static double mass_to_radius(double mass) {
            double log_mass = log10(mass/10e22);
            return (log_mass >=0 ? log_mass : 0) + 1;
        }
#if DATA_ASTRE_HOLDS_TRAJECTORY
        orbit::OrbitalTrajectory& getTrajectory() const { return *this->trajectory; }
        void setTrajectory(orbit::OrbitalTrajectory t)
                                    { this->trajectory = new orbit::OrbitalTrajectory(t); }
        bool haveTrajectory() const { return this->trajectory != NULL; }
#endif
#if VIEW_INITIAL_POSITION
        double getInitX() const { return this->init_position_x; }
        double getInitY() const { return this->init_position_y; }
#endif


    protected:
        // Graphic
        void mousePressEvent(QGraphicsSceneMouseEvent*);


    private:
        void init_attributes(double, double, double, double, double, double, std::string, QColor);

        double mass, radius; // mass in kg, radius in pixel
        double position_x, position_y; // position in astronomic unit (AU)
        double speed_x, speed_y; // speed in meter per second (m.s-1)
        double accel_x, accel_y;
        bool visible;
        bool nullified;
        std::string name;
        QColor color;
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

} // namespace view
