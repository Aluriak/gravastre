#include "engine.cpp"
#include "view.h"


view::Universe::Universe(eng::Engine& engine, const utils::JsonConfig& config,
                         bool start_paused, QWidget* parent) :
                         QGraphicsView(parent), engine(engine), pause(start_paused) {
#if ALLOW_PLAYER
    // Player
    this->player = config.getPlayers().size() > 0 ? config.getPlayer(0) : nullptr;
    if(config.getPlayers().size() > 1) {
        std::cout << "Multiple players detected. Only '"
                  << this->player->name << "' will be used." << std::endl;
    }
    if(this->player) {
        std::cout << "Universe player: " <<this->player << " > '"
                  << this->player->name << "'" << std::endl;
    }
#endif
    // Scene config
    this->scene = new QGraphicsScene;
    this->setScene(this->scene);
    this->setInteractive(true);
    this->setMouseTracking(true);
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
    this->scene->setBackgroundBrush(QBrush(Qt::black));
    // Items config
#if VIEW_VISIBLE_REFERENCE
    this->reference.setRect(-2, -2, 4, 4);
    this->reference.setPos(0, 0);
    this->reference.setBrush(QBrush(Qt::white));
    this->reference.setPen(QPen(Qt::white));
#else
    this->reference.setRect(0, 0, 0, 0);
    this->reference.setPos(0, 0);
    this->reference.setBrush(QBrush(Qt::transparent));
    this->reference.setPen(QPen(Qt::transparent));
#endif
    this->placement_line.setPen(QPen(Qt::blue));
    this->placement_line.setVisible(false);
    this->placement_line.setParentItem(&this->reference);
    this->scene->addItem(&reference);
    this->selected_object = nullptr;

    // Register all Astre as graphic object, prepare the view.
    eng::Interactant* maximal_mass = NULL;  // will center the view on the heavier astre
    for(auto astre : this->engine.getInteractants()) {
        this->add_astre_to_graphic(astre);
        if(maximal_mass == NULL or astre->getMass() > maximal_mass->getMass()) {
            maximal_mass = astre;
        }
    }
    if(maximal_mass != NULL) {
        //this->centerOn(maximal_mass->getX(), maximal_mass->getY());  // TODO
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
    // Random color
    QColor random_color(utils::randnum(0, 255), utils::randnum(0, 255), utils::randnum(0, 255));
    // Create the astre
    this->add_astre(new eng::Astre(mass, x, y, vx, vy, "unamed", random_color));
}
void view::Universe::add_astre(eng::Interactant* interactant) {
    this->engine.add(interactant);
    this->add_astre_to_graphic(interactant);
}


/**
 * [SLOT] Compute the next step and print it.
 */
void view::Universe::update_engine() {
    if(not this->pause) {
        this->engine.update();
        // View is centered on selected object
        if(this->follow_selection) {
            assert(this->selected_object);
            double x = this->selected_object->pos().x();
            double y = this->selected_object->pos().y();
            this->setSceneRect(
                x - (double)this->width()  / 2.,
                y - (double)this->height() / 2.,
                x + (double)this->width() * 2.5,
                y + (double)this->height() * 2.5
            );
            this->centerOn(x, y);
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



#if ALLOW_PLAYER
/**
 * Spawn the player space ship
 */
eng::Interactant* view::Universe::spawn_ship(double x, double y, double vx, double vy) {
    assert(this->player != nullptr);
    assert(ship == nullptr);
    this->ship = new eng::PlayerShip(1e5, 6, x, y, vx, vy, this->player->name,
                                     QColor(255, 255, 255));
    return this->ship;
}
#endif


/**
 * Set given interactant as the one to be followed
 */
void view::Universe::select(eng::Interactant* interactant) {
#if VIEW_TRAJECTORY_DATA_ON_SELECTED
    // change objects states
    if(this->selected_object != nullptr)
        this->selected_object->setDrawVelocity(false);
    if(interactant != nullptr)
        interactant->setDrawVelocity(true);
#endif
    // change this state
    this->follow_selection = interactant != nullptr;
    this->selected_object = interactant;
    this->reference.setPos(0, 0);
}



void view::Universe::mousePressEvent(QMouseEvent* event) {
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
            if(clicked != &this->reference) {
                // the item is necessarily an Interactant.
                this->select((eng::Interactant*) clicked);
            }
            std::cerr << "Item selected has changed to " << clicked << "\n";
#if VIEW_DETAILS_ON_MOUSE_CLIC
            // TODO: print informations on astre and its trajectory
#endif
        }
    } else if(event->button() == Qt::RightButton) {
        this->drag_translation_x = scene_coords.x();
        this->drag_translation_y = scene_coords.y();
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

#if ALLOW_PLAYER
        if(mode_spawn_ship) {
            mode_spawn_ship = false;
            this->add_astre(this->spawn_ship(
                unit::pixel_to_au(line.x1()),
                unit::pixel_to_au(line.y1()),
                unit::kilometer_to_meter(this->selected_speed * speed_x),
                unit::kilometer_to_meter(this->selected_speed * speed_y)
            ));
            std::cout << "Player ship spawned." << std::endl;
            this->select(ship);
        } else
#endif
        {  // spawn a regular astre
            this->add_astre(
                this->selected_mass,
                unit::pixel_to_au(line.x1()),
                unit::pixel_to_au(line.y1()),
                unit::kilometer_to_meter(this->selected_speed * speed_x),
                unit::kilometer_to_meter(this->selected_speed * speed_y)
            );
        }
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
    } else if(event->key() == Qt::Key_S) {
#if ALLOW_PLAYER
        if(this->player != nullptr) {
            if(this->ship != nullptr) {
                this->engine.remove(this->ship);
                ship = nullptr;
                std::cout << "Player ship destroyed." << std::endl;
            } else if(this->mode_spawn_ship) {
                std::cout << "Player mode deactivated." << std::endl;
                mode_spawn_ship = false;
            } else {
                std::cout << "Player mode activated." << std::endl;
                mode_spawn_ship = true;
            }
        } else {
            std::cout << "No player available" << std::endl;
        }
    } else if(event->key() == Qt::Key_Right) {
        if(ship != nullptr) ship->moveRight();
    } else if(event->key() == Qt::Key_Left) {
        if(ship != nullptr) ship->moveLeft();
    } else if(event->key() == Qt::Key_Up) {
        if(ship != nullptr) ship->moveUp();
    } else if(event->key() == Qt::Key_Down) {
        if(ship != nullptr) ship->moveDown();
#else
        std::cout << "Not compiled with player support." << std::endl;
#endif
    } else if(event->key() == Qt::Key_P) {
        this->togglePause();
        std::cout << (this->pause?"Paused":"Running") << std::endl;
    } else if(event->key() == Qt::Key_Space) {
        this->reference.setPos(0, 0);
        this->follow_selection = not this->follow_selection;
    } else if(event->key() == Qt::Key_Escape) {
        QCoreApplication::quit();
    }
}
