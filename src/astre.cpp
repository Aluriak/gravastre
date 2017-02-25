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
    painter->drawEllipse(QPointF(0, 0), radius_px, radius_px);
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
void eng::Astre::init_attributes(const QColor new_color, const double new_radius_km) {
#if ALWAYS_USE_DEFAULT_RADIUS
    this->radius_km = -1;
    this->radius_px = Astre::mass_to_radius(this->getMass());
    std::cout << this->getName() << " use default radius: " << this->radius_px << "px" << std::endl;
#else
    if(new_radius_km > 0) {  // use the brand new one radius in km if given
        this->radius_km = new_radius_km;
    }
    if(this->radius_km > 0.) {
        std::cout << this->getName() << " use given radius: " << this->radius_km;
        this->radius_px = Astre::km_to_pixel_radius(this->radius_km);
        std::cout << " -> " << this->radius_px << std::endl;
    } else {  // no radius in km available: let's use the mass.
        this->radius_px = Astre::mass_to_radius(this->getMass());
        std::cout << this->getName() << " use default radius: " << this->radius_px << "px" << std::endl;
    }
#endif

    // Bounding rectangle, needed by QPainter.
    qreal penWidth = 1;
    this->drawing_rect = QRectF(-radius_px - penWidth / 2., -radius_px - penWidth / 2.,
                                radius_px + penWidth, radius_px + penWidth);
    // graphics color
    this->color = new_color != nullptr ? new_color : this->color;
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}
