#include "drawableVelocity.h"


view::DrawableVelocity::DrawableVelocity(eng::Interactant* inter) : interactant(inter) {}



void view::DrawableVelocity::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                                   QWidget*widget) {
    // the whole reason we have to export velocity drawing outside the interactant class:
    this->setRotation(-this->interactant->rotation());
    // paint the acceleration
    QColor pcolor(255, 0, 0);
    painter->setBrush(QBrush(pcolor));
    painter->setPen(QPen(pcolor));  // outline color
    painter->drawLine(interactant->getX(), interactant->getY(),
                      interactant->getX() + 1.5*interactant->getAccelX(),
                      interactant->getY() + 1.5*interactant->getAccelY());
    // paint the speed
    pcolor = QColor(0, 255, 0);
    painter->setBrush(QBrush(pcolor));
    painter->setPen(QPen(pcolor));  // outline color
    painter->drawLine(interactant->getX(), interactant->getY(),
                      interactant->getX() + 1.5*unit::meter_to_kilometer(interactant->getSpeedX()),
                      interactant->getY() + 1.5*unit::meter_to_kilometer(interactant->getSpeedY()));
}
