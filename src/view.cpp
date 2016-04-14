#include "engine.cpp"
#include "view.h"


view::Universe::Universe(eng::Engine& engine, QWidget* parent) :
                         QGraphicsView(parent), engine(engine) {
    // Scene config
    this->scene = new QGraphicsScene;
    this->setScene(this->scene);
    this->setInteractive(true);
    this->setMouseTracking(true);
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->scene->setBackgroundBrush(QBrush(Qt::black));
    this->setSceneRect(
        -this->width(),
        -this->height(),
        this->width() * 3.,
        this->height() * 3.
    );
    // Items config
    this->reference.setRect(-2, -2, 4, 4);
    this->reference.setPos(0, 0);
    this->reference.setBrush(QBrush(Qt::white));
    this->reference.setPen(QPen(Qt::white));
    this->reference.setVisible(VIEW_VISIBLE_REFERENCE);
    this->placement_line.setPen(QPen(Qt::blue));
    this->placement_line.setVisible(false);
    this->placement_line.setParentItem(&this->reference);
    this->scene->addItem(&reference);
    this->selected_object = &this->reference;

    // Register all Astre as graphic object, prepare the view.
    eng::Astre* maximal_mass = NULL;  // will center the view on the heavier astre
    for(auto astre : this->engine.getAstres()) {
        this->add_astre(astre);
        if(maximal_mass == NULL or astre->getMass() > maximal_mass->getMass()) {
            maximal_mass = astre;
        }
    }
    if(maximal_mass != NULL) {
        //this->centerOn(maximal_mass->getX(), maximal_mass->getY());
    }
    this->centerOn(0, 0);

    // Data
    this->selected_mass = 1e3;
    this->selected_speed = 1e-1;
    this->drag_translation_x = 0.;
    this->drag_translation_y = 0.;

    // Configure the timer
    connect(&this->update_call, SIGNAL(timeout()), this, SLOT(update_engine()));
    update_call.start(VIEW_ENGINE_TIME_UPDATE);
}


/**
 * Add a new Astre with given data in engine, and give it a view.
 */
void view::Universe::add_astre(double mass, double x, double y, double vx, double vy) {
    this->add_astre(this->engine.add_astre(mass, x, y, vx, vy));
}
void view::Universe::add_astre(eng::Astre* astre) {
    astre->setParentItem(&this->reference);
}


/**
 * [SLOT] Compute the next step and print it.
 */
void view::Universe::update_engine() {
    if(not this->pause) {
        this->engine.update();
        if(this->follow_selection) {
            this->centerOn(this->selected_object->pos().x(),
                           this->selected_object->pos().y());
        }
    }
    this->update();
    this->scene->update();
}

/**
 * [SLOT] Translate all scene items.
 */
void view::Universe::translate(double x, double y) {
    this->reference.moveBy(
        (VIEW_REVERSED_TRANSLATION? -1:1) * x * VIEW_TRANSLATION_RATIO,
        (VIEW_REVERSED_TRANSLATION? -1:1) * y * VIEW_TRANSLATION_RATIO
    );
}

//void view::Universe::paintEvent(QPaintEvent*) {
    //QPen default_pen(Qt::transparent);
    //QPainter painter(this);
    //painter.setBrush(QBrush(Qt::black));  // fill color
    //painter.setPen(default_pen);  // outline color
    //painter.drawRect(rect());
    //painter.translate(this->drag_translation_x, this->drag_translation_y);

    //if(this->drag_placement) {
        //painter.setPen(QPen(Qt::blue));
        //painter.drawLine(this->placement_line);
        //painter.setPen(default_pen);
    //}

    //// print astres
    //QPoint astre_center;
    ////eng::Engine::AstreIterator it = this->engine.begin();
    ////for(; it != this->engine.end() ; it++) {
    //for(auto it : this->engine.getAstres()) {
        //eng::Astre* astre = it;
        //astre_center.setX(unit::au_to_pixel(astre->getX()));
        //astre_center.setY(unit::au_to_pixel(astre->getY()));
        //painter.setBrush(QBrush(astre->getColor()));  // fill color
        //painter.setPen(QPen(Qt::transparent));  // outline color
        //painter.drawEllipse(
            //astre_center,
            //(int)astre->getRadius(),
            //(int)astre->getRadius()
        //);
//#if VIEW_INITIAL_POSITION
        //painter.setBrush(QBrush(Qt::red));  // fill color
        //painter.setPen(QPen(Qt::red));  // outline color
        //painter.drawEllipse(QPoint(unit::au_to_pixel(astre->getInitX()),
                            //unit::au_to_pixel(astre->getInitY())), 2, 2);
//#endif
//#if VIEW_ORBITAL_TRAJECTORY
        //if(astre->haveTrajectory() != NULL) {
            //const int OFFSET = 1;  // center of all trajectories printing
            //painter.setBrush(QBrush(Qt::transparent));  // fill color
            //painter.setPen(QPen(Qt::blue));  // outline color
            //painter.drawEllipse(
                //QPoint(unit::au_to_pixel(OFFSET), unit::au_to_pixel(OFFSET)),
                //(int)unit::meter_to_pixel(astre->getTrajectory().getWidth()),
                //(int)unit::meter_to_pixel(astre->getTrajectory().getHeight())
            //);
        //}
//#endif
    //}
//}


void view::Universe::mousePressEvent(QMouseEvent* event) {
    //std::cout << "Clic detected at:" << std::endl
              //<< "\tevent->pos():" << event->pos().x() << ";" << event->pos().y() << std::endl
              //<< "\tevent->localPos():" << event->localPos().x() << ";" << event->localPos().y() << std::endl
              //<< "\tevent->globalPos():" << event->globalPos().x() << ";" << event->globalPos().y() << std::endl
              //<< "\tevent->screenPos():" << event->screenPos().x() << ";" << event->screenPos().y() << std::endl
              //<< "\tevent->windowPos():" << event->windowPos().x() << ";" << event->windowPos().y() << std::endl
              //<< "\tScreen:" << event->screenPos().x() << ";" << event->screenPos().y() << std::endl
              //<< "\tScene:" << scene_coords.x() << ";" << scene_coords.y() << std::endl
              //<< "\tEND"  << std::endl;
#if VIEW_DETAILS_ON_MOUSE_CLIC
    bool astre_found = false;
    //for(auto it : this->engine.getAstres()) {
    //}  // TODO
    if(not astre_found) {
#else
    if(true) {
#endif
        QPointF scene_coords = this->mapToScene(QPoint(event->x(), event->y()));
        if(event->button() == Qt::LeftButton) {
            QGraphicsItem* clicked = this->scene->itemAt(scene_coords, QTransform());
            if(clicked == NULL) {
                this->placement_line.setVisible(true);
                this->placement_line.setLine(
                    scene_coords.x() - this->reference.pos().x(),
                    scene_coords.y() - this->reference.pos().y(),
                    scene_coords.x() - this->reference.pos().x(),
                    scene_coords.y() - this->reference.pos().y()
                );
            } else { // an item was clicked
                this->selected_object = clicked;
            }
        } else if(event->button() == Qt::RightButton) {
            this->drag_translation_x = scene_coords.x();
            this->drag_translation_y = scene_coords.y();
        }
    }
    // Call default implementation, to propagate to the graphicsitems.
    QGraphicsView::mousePressEvent(event);
}


void view::Universe::mouseMoveEvent(QMouseEvent* event) {
    QPointF scene_coords = this->mapToScene(QPoint(event->x(), event->y()));
    if(this->placement_line.isVisible()) {
        this->placement_line.setLine(
            this->placement_line.line().x1(),
            this->placement_line.line().y1(),
            scene_coords.x() - this->reference.pos().x(),
            scene_coords.y() - this->reference.pos().y()
        );
    } else if(event->buttons() & Qt::RightButton) {
        this->translate(scene_coords.x() - this->drag_translation_x,
                        scene_coords.y() - this->drag_translation_y);
        this->drag_translation_x = scene_coords.x();
        this->drag_translation_y = scene_coords.y();
    }
    // Call default implementation
    QGraphicsView::mouseMoveEvent(event);
}


void view::Universe::mouseReleaseEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton && this->placement_line.isVisible()) {
        this->placement_line.setVisible(false);
        QLineF line = this->placement_line.line();
        double speed_x = line.x2() - line.x1();
        double speed_y = line.y2() - line.y1();
        this->add_astre(
            this->selected_mass,
            unit::pixel_to_au(line.x1()),
            unit::pixel_to_au(line.y1()),
            unit::kilometer_to_meter(this->selected_speed * speed_x),
            unit::kilometer_to_meter(this->selected_speed * speed_y)
        );
        this->placement_line.setVisible(false);
    }
    // Call default implementation
    QGraphicsView::mouseReleaseEvent(event);
}


void view::Universe::wheelEvent(QWheelEvent* event) {
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;
    if (event->delta()>0) {
        this->scale(scaleFactor,scaleFactor);
    } else {
        this->scale (1. / scaleFactor, 1. / scaleFactor);
    }
}


void view::Universe::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_E) {
        this->selected_mass *= 10;
        std::cout << "selected mass increased to " << this->selected_mass << std::endl;
    } else if(event->key() == Qt::Key_I) {
        this->selected_mass /= 10;
        std::cout << "selected mass decreased to " << this->selected_mass << std::endl;
    } else if(event->key() == Qt::Key_D) {
        std::cout << "universe cleared !" << std::endl;
        this->engine.clear_all();
    } else if(event->key() == Qt::Key_U) {
        this->selected_speed *= 10;
        std::cout << "selected speed increased to " << this->selected_speed << std::endl;
    } else if(event->key() == Qt::Key_A) {
        this->selected_speed /= 10;
        std::cout << "selected speed decreased to " << this->selected_speed << std::endl;
    } else if(event->key() == Qt::Key_P) {
        this->togglePause();
        std::cout << (this->pause?"Paused":"Running") << std::endl;
    } else if(event->key() == Qt::Key_Space) {
        this->reference.setPos(0, 0);
        this->follow_selection = true;
    }
}
