#pragma once

#include <iostream>
#include <list>
#include <assert.h>
#include <cmath>
#include "converters.h"
#include "astre.h"




namespace eng {

    class Engine {

    public:
        //Engine();
        void add_astre(double, double, double, double, double,
                       std::string="unamed", QColor=Qt::white);
        void update();
        void clear_all();

        typedef std::list<Astre*>::iterator AstreIterator;
        AstreIterator begin() { return astres.begin(); };
        AstreIterator end() { return astres.end(); };

    private:
        std::list<Astre*> astres;

    };

} // namespace view
