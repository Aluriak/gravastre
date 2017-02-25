#include "astre.h"


/**
 * Object lifetime
 */
eng::Astre::Astre(double mass, double radius_km, double pos_x, double pos_y,
                  double speed_x, double speed_y, std::string name, QColor color) :
    Interactant(mass, pos_x, pos_y, speed_x, speed_y, name)
{
    this->init_attributes(color, radius_km);
}

// Version without radius: deduce radius from mass
eng::Astre::Astre(double mass, double pos_x, double pos_y,
                  double speed_x, double speed_y, std::string name, QColor color) :
    Interactant(mass, pos_x, pos_y, speed_x, speed_y, name)
{
    this->init_attributes(color);
}

eng::Astre::~Astre() {}




/**
 * Draw an Astre
 */
void eng::Astre::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
    painter->setBrush(QBrush(this->color));
    painter->setPen(QPen(color));  // outline color
    painter->drawEllipse(-radius, -radius, radius, radius);
}



/**
 * Absorbtion needs to be reupdated
 */
void eng::Astre::absorbs(eng::Interactant* const othr) {
    Interactant::absorbs(othr);
    this->init_attributes();
}




/**
 * [GRAPHICS] Callback when the Astre is clicked.
 */
void eng::Astre::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    std::cerr << "Clic on " << this->getName() << std::endl;
}


/**
 * PRIVATE: Initialize fields. Called by constructors.
 * if radius is valid, it will override the radius.
 */
void eng::Astre::init_attributes(const QColor new_color, const double new_radius) {
    this->radius = new_radius > 0 ? new_radius : Astre::mass_to_radius(this->getMass());
    qreal penWidth = 1;
    this->drawing_rect = QRectF(-radius - penWidth / 2., -radius - penWidth / 2.,
                                radius + penWidth, radius + penWidth);
    // graphics color
    this->color = new_color != nullptr ? new_color : this->color;
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}
