#include "player.h"


/**
 * Object lifetime
 */
eng::PlayerShip::PlayerShip(double mass, double size, double pos_x, double pos_y,
                            double speed_x, double speed_y, std::string name, QColor color) :
    Interactant(mass, pos_x, pos_y, speed_x, speed_y, name)
{
    this->init_attributes(color, size);
}

// Version without size: deduce size from mass
eng::PlayerShip::PlayerShip(double mass, double pos_x, double pos_y,
                  double speed_x, double speed_y, std::string name, QColor color) :
    Interactant(mass, pos_x, pos_y, speed_x, speed_y, name)
{
    this->init_attributes(color);
}

eng::PlayerShip::~PlayerShip() {}




/**
 * Draw an PlayerShip
 */
void eng::PlayerShip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget) {
    Interactant::paint(painter, option, widget);
    // paint the ship itself
    painter->setBrush(QBrush(this->color));
    painter->setPen(QPen(this->color));  // outline color
    painter->drawPolygon(this->drawing_shape);
}



/**
 * Absorbtion needs to be reupdated
 */
void eng::PlayerShip::absorbs(eng::Interactant* const othr) {
    Interactant::absorbs(othr);
    this->init_attributes();
}



/**
 * [MOVEMENT] Thrust control
 */
void eng::PlayerShip::moveUp() {
    this->thrust = thrust+5;
    if(this->thrust > max_thrust) thrust = max_thrust;
    if(this->thrust < min_thrust) thrust = min_thrust;
    mylog();
}
void eng::PlayerShip::moveDown() {
    this->thrust = thrust-10;
    if(this->thrust > 100) thrust = 100;
    if(this->thrust <   0) thrust =   0;
    mylog();
}
/**
 * [MOVEMENT] churn
 */
void eng::PlayerShip::moveRight() {
    this->setRotation(this->rotation() + 5);
    mylog();
}
void eng::PlayerShip::moveLeft() {
    this->setRotation(this->rotation() - 5);
    mylog();
}



/**
 * [GRAPHICS] Callback when the PlayerShip is clicked.
 */
void eng::PlayerShip::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    std::cerr << "Clic on " << this->getName() << std::endl;
}


/**
 * PRIVATE: Initialize fields. Called by constructors.
 * if size is valid, it will override the size.
 */
void eng::PlayerShip::init_attributes(const QColor new_color, const double new_size) {
    this->size = new_size > 0 ? new_size : PlayerShip::mass_to_size(this->getMass());
    this->drawing_rect = this->drawing_shape.boundingRect();
    // Create the shape
    QPolygonF triangle;
    QPointF first(-this->getSize(), 0);
    triangle << first << QPointF(this->getSize(), 0) << QPointF(0, this->getSize() * 2.5) << first;
    this->drawing_shape = triangle;
    assert(this->drawing_shape.isClosed());
    // graphics color
    this->color = new_color != nullptr ? new_color : this->color;
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}
