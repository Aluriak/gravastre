#pragma once


#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <QColor>
#include <QPainter>

#include "interactant.h"
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
     * An Astre is the figuration of planets, stars, and anything that have a spherical form
     */
    class Astre : public Interactant {

    public:
        // Lifetime
        Astre(double, double, double, double, double, double, std::string, QColor);
        Astre(double, double, double, double, double, std::string, QColor);
        virtual ~Astre();

        // Methods
        void absorbs(eng::Interactant* const);

        // Class Methods
        static double mass_to_radius(double mass) {  // returned radius in km
            double log_mass = log10(mass/DEFAULT_RADIUS_BY_MASS_FACTOR);
            return (log_mass >=0 ? log_mass : 0) + 1;
        }
        static double km_to_pixel_radius(double radius_km) {
            double radius_pixel = unit::kilometer_to_pixel(radius_km) * RADIUS_TO_PIXEL_FACTOR;
            return radius_pixel;
        }

        // Accessors
        double getRadius() const { return this->radius_px; }
        QColor getColor() const { return this->color; }
        QRectF boundingRect() const { return this->drawing_rect; };


    protected:
        // Graphic
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);


    private:
        // Methods
        void init_attributes(const QColor=nullptr, const double=-1);

        // Attributes
        double radius_km = -1; // radius in km
        double radius_px = -1; // radius in pixel
        QColor color;
        QRectF drawing_rect;

    };

} // namespace eng
