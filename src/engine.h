#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <tuple>

#include "converters.h"
#include "system.h"
#include "interactant.h"
#include "astre.h"




namespace eng {

    class Engine {

    public:
        //Engine();
        Interactant* add(Interactant* astre);
        Interactant* add(AstreData, PositionAndSpeed);
        Interactant* add(double, double, double, double, double,
                         std::string="unamed", QColor=Qt::white);
        Interactant* add(double, double, double, double, double, double,
                         std::string="unamed", QColor=Qt::white);
        void spawn(System&, std::tuple<double, double> =std::make_tuple(0, 0));
        void spawn(System&, PositionAndSpeed);
        void update();
        bool remove(Interactant * const, bool delete_it=true);
        void clear_all();

        std::list<Interactant*>::iterator begin() { return astres.begin(); };
        std::list<Interactant*>::iterator end() { return astres.end(); };
        std::list<Interactant*> getInteractants() const { return this->astres; }

    private:
        std::list<Interactant*> astres;

    };

} // namespace view
