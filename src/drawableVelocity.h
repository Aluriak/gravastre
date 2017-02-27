#pragma once



#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <QGraphicsItem>
#include <QPainter>
#include <QColor>

#include "interactant.h"
#include "converters.h"
#include "orbit.h"


namespace eng {class Interactant;}
namespace view {

    /**
     * Holded by Interactants objects for print their velocity.
     */
    class DrawableVelocity : public QGraphicsItem {

    public:
        // Lifetime
        DrawableVelocity(eng::Interactant*);
        ~DrawableVelocity() {}

        // Accessors
        QRectF boundingRect() const { return QRectF(-10, -10, 20, 20); };

    protected:
        // Methods
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);


    private:
        // Attributes
        eng::Interactant * const interactant;
        const QColor color_speed = QColor(0, 255, 0);
        const QColor color_accel = QColor(255, 0, 0);

    };

} // namespace view
