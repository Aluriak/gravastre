#pragma once

#include <assert.h>
#include <cmath>
#include <iostream>
#include <list>
#include <tuple>

#include "converters.h"
#include "system.h"
#include "astre.h"




namespace eng {

    class Engine {

    public:
        //Engine();
        Astre* add_astre(Astre* astre);
        Astre* add_astre(AstreData, PositionAndSpeed);
        Astre* add_astre(double, double, double, double, double,
                         std::string="unamed", QColor=Qt::white);
        Astre* add_astre(double, double, double, double, double, double,
                         std::string="unamed", QColor=Qt::white);
        void spawn(System&, std::tuple<double, double> =std::make_tuple(0, 0));
        void spawn(System&, PositionAndSpeed);
        void update();
        void clear_all();

        std::list<Astre*>::iterator begin() { return astres.begin(); };
        std::list<Astre*>::iterator end() { return astres.end(); };
        std::list<Astre*> getAstres() const { return this->astres; }

    private:
        std::list<Astre*> astres;

    };

} // namespace view
