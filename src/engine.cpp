#include "engine.h"


void eng::Engine::add_astre(double mass, double pos_x, double pos_y,
                            double speed_x, double speed_y,
                            std::string name, QColor color) {
    assert(mass > 0.);

    double radius = Astre::mass_to_radius(mass);
    double speed_x_px = unit::kilometer_to_meter(speed_x);  // km.s-1 to m.s-1
    double speed_y_px = unit::kilometer_to_meter(speed_y);  // (Astre expects m.s-1)

    this->astres.push_back(new Astre(
        mass, radius,
        pos_x, pos_y,
        speed_x_px, speed_y_px,
        name, color
    ));
    std::cout << "Creation of " << name << ", new Astre at" << std::endl
              << "\tmass=" << mass << "; radius=" << radius << std::endl
              << "\tposition=(" << pos_x << ";"
                                << pos_y << ")" << std::endl
              << "\tspeed=(" << speed_x << ";" << speed_y << ")" << std::endl
              << "\tpx_speed=(" << speed_x_px << ";" << speed_y_px << ")" << std::endl;
}


void eng::Engine::update() {
    //std::cout << "engine updated !" << std::endl;
    std::list<Astre*>::iterator it_astres = this->astres.begin();
    std::list<Astre*>::iterator it_interactor = this->astres.begin();
    // make the astre consider themselves, and handle collisions
    for(; it_astres != this->astres.end(); it_astres++) {
        it_interactor = it_astres;
        it_interactor++;  // don't perform interactions between same astres
        for(; it_interactor != this->astres.end(); it_interactor++) {
            double dist = (*it_astres)->distTo(*it_interactor);
            assert(*it_interactor != *it_astres);
            if((*it_astres)->updatable() and (*it_interactor)->updatable()) {
                if((*it_astres)->collide(*it_interactor, dist)) {
                    //std::cout << "Collision detected !" << std::endl;
                    // the bigger absorbs the ligher
                    if((*it_astres)->bigger(*it_interactor)) {
                        (*it_astres)->absorbs(*it_interactor);
                    } else {
                        (*it_interactor)->absorbs(*it_astres);
                    }
                } else {
                    (*it_astres)->accelerateTo(*it_interactor, dist);
                    (*it_interactor)->accelerateTo(*it_astres, dist);
                } // astre in collision or totally used
            } // astre not updatable
        }
    }
    // update all astres
    it_astres = this->astres.begin();
    for(; it_astres != this->astres.end(); it_astres++) {
        if((*it_astres)->updatable()) {
            (*it_astres)->update();
        } else {  // astre was destroyed, removed,…
            delete (*it_astres);
            it_astres = this->astres.erase(it_astres);
        }
    }
    // debug
    //std::cout << "Astre count: " << this->astres.size() << std::endl;
}


void eng::Engine::clear_all() {
    // first, nullify them all for avoid any side effect on concurrent
    //  call on the list.
    std::list<Astre*>::iterator it_astres = this->astres.begin();
    for(; it_astres != this->astres.end(); it_astres++) {
        (*it_astres)->nullify();
    }
    // then free then all.
    it_astres = this->astres.begin();
    for(; it_astres != this->astres.end(); it_astres++) {
        delete (*it_astres);
        it_astres = this->astres.erase(it_astres);
    }
}
