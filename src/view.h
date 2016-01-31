#pragma once


#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <iostream>
#include "engine.h"
#include "astre.h"


namespace view {

    class Universe : public QWidget {
    Q_OBJECT

    public slots:
        void update_engine();

    public:
        Universe(eng::Engine&, QWidget *parent = 0);

        virtual void paintEvent(QPaintEvent*);
        virtual void mousePressEvent(QMouseEvent*);
        virtual void mouseMoveEvent(QMouseEvent*);
        virtual void mouseReleaseEvent(QMouseEvent*);
        virtual void keyPressEvent(QKeyEvent*);

    private:
        QPoint global_to_relative(const QPoint& point) const;
        QPoint global_to_relative(const QPointF& point) const;

        QLine placement_line;
        QPainter painter;
        QTimer update_call;
        bool drag_placement, drag_translation;
        double drag_translation_x, drag_translation_y;
        double space_ref_x, space_ref_y;
        double selected_mass;
        double selected_speed;  // the bigger is, the speedy will be the new astre
        eng::Engine& engine;

    };

} // namespace view
