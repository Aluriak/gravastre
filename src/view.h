#pragma once


#include <iostream>
#include <tuple>

#include <QCoreApplication>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QLineF>
#include <QList>

#include "engine.h"
#include "interactant.h"
#include "astre.h"
#include "system.h"
#include "player.h"



namespace view {

    class Universe : public QGraphicsView {
    Q_OBJECT

    public slots:
        void update_engine();
        void translate(double x, double y);

    public:
        // Lifetime
        Universe(eng::Engine&, const utils::JsonConfig& config, bool start_paused=false,
                 QWidget* parent=NULL);

        // Methods
        void add_astre_to_graphic(eng::Interactant* inter) { inter->setParentItem(&this->reference); };
        void add_astre(eng::Interactant*);
        void add_astre(double, double, double, double, double);
        void togglePause() { this->pause = not this->pause; }
#if ALLOW_PLAYER
        eng::Interactant* spawn_ship(double, double, double, double);
#endif
        void select(eng::Interactant*);

        virtual void mousePressEvent(QMouseEvent*);
        virtual void mouseMoveEvent(QMouseEvent*);
        virtual void mouseReleaseEvent(QMouseEvent*);
        virtual void wheelEvent(QWheelEvent*);
        virtual void keyPressEvent(QKeyEvent*);


    private:
        QGraphicsScene* scene;
        QGraphicsRectItem reference;  // Parent of all other scene items (astre, lines,…).
                                      // Needed to perform translation.
        QGraphicsLineItem placement_line;
        QTimer update_call;
        double drag_translation_x, drag_translation_y;
        eng::Interactant* selected_object;
        double selected_mass;
        double selected_speed;  // the bigger is, the speedy will be the new astre
        eng::Engine& engine;
        bool pause;  // true: no engine update
        bool follow_selection = false;  // true: keep selected object in view center
#if ALLOW_PLAYER
        bool mode_spawn_ship  = false; // true: next left clic will launch a player ship
        eng::PlayerShip* ship = nullptr;
        utils::Player* player;
#endif

    };

} // namespace view
