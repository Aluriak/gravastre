#pragma once


#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <QColor>
#include <QPainter>

#include "interactant.h"




namespace eng {

    /**
     * Object evolving in 2D space, interacting gravitationnaly with the others.
     */
    class PlayerShip : public Interactant {

    public:
        // Lifetime
        PlayerShip(double, double, double, double, double, double, std::string, QColor);
        PlayerShip(double, double, double, double, double, std::string, QColor);
        virtual ~PlayerShip();

        // Methods
        void absorbs(eng::Interactant* const);
        void moveUp();
        void moveDown();
        void moveRight();
        void moveLeft();

        // Accessors
        double getSize() const { return this->size; }
        QColor getColor() const { return this->color; }
        QRectF boundingRect() const { return this->drawing_rect; };
        double getBasicAccelX() const { return -thrust * round(cos(unit::degree_to_radian(rotation() - 90))); }
        double getBasicAccelY() const { return -thrust * round(sin(unit::degree_to_radian(rotation() - 90))); }

        void mylog() const {
        std::cout << "ANGLE:" << this->rotation() << std::endl;
        std::cout << "THRUST:" << this->thrust << std::endl;
        std::cout << "SPEED: (" << this->getSpeedX() << ";" << this->getSpeedY() << ")" << std::endl;
        std::cout << "COS: " << cos(unit::degree_to_radian(rotation() - 90)) << std::endl;
        std::cout << "SIN: " << sin(unit::degree_to_radian(rotation() - 90)) << std::endl;
        std::cout << "NCOS: " << (unit::degree_to_radian(rotation() - 90)) << std::endl;
        std::cout << "NSIN: " << (unit::degree_to_radian(rotation() - 90)) << std::endl;
        std::cout << "RCOS: " << round(cos(unit::degree_to_radian(rotation() - 90))) << std::endl;
        std::cout << "RSIN: " << round(sin(unit::degree_to_radian(rotation() - 90))) << std::endl;
        }

        // Class Methods
        static double mass_to_size(double mass) {
            double log_mass = log10(mass/10e6);
            return (log_mass >=0 ? log_mass : 0) + 1;
        }


    protected:
        // Graphic
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*); // inherited


    private:
        // Methods
        void init_attributes(const QColor=nullptr, const double=-1);

        // Attributes
        double size; // in pixel
        QColor color;
        QRectF drawing_rect;
        QPolygonF drawing_shape;
        const double min_thrust = 0., max_thrust = 100.; // newton
        double thrust = 0; // newton ; added to acceleration

    };

} // namespace view
