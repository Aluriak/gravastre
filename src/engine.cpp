#include "engine.h"


eng::Interactant* eng::Engine::add(eng::Interactant* astre) {
    assert(astre != NULL);
    this->astres.push_back(astre);
#if DATA_ASTRE_HOLDS_SYSTEM
    astre->setTrajectory(system.getTrajectory());
#endif
#if DEBUG_CREATION_LOGS
    std::cerr << "Creation of " << astre->getName() << ", new Interactant at" << std::endl
              << "\tmass=" << astre->getMass()
              << "; radius=" << astre->getSize() << std::endl
              << "\tposition=(" << astre->getX() << ";"
                                << astre->getY() << ")" << std::endl
              << "\tspeed=(" << astre->getSpeedX() << ";"
                             << astre->getSpeedY() << ")" << std::endl;
#endif
    return astre;
}


eng::Interactant* eng::Engine::add(eng::AstreData astre,
                                   eng::PositionAndSpeed pos_speed) {
    return this->add(
        std::get<0>(astre),  // mass
        std::get<1>(astre),  // radius
        std::get<0>(pos_speed), std::get<1>(pos_speed),  // position
        std::get<2>(pos_speed), std::get<3>(pos_speed),  // speed
        std::get<2>(astre),  // name
        std::get<3>(astre)  // color
    );
}


// no radius in this one
eng::Interactant* eng::Engine::add(double mass, double pos_x, double pos_y,
                                   double speed_x, double speed_y,
                                   std::string name, QColor color) {
    assert(mass > 0.);
    return this->add(
        new Astre(
            mass,
            pos_x, pos_y,
            speed_x, speed_y,
            name, color
        )
    );
}


eng::Interactant* eng::Engine::add(double mass, double radius, double pos_x,
                                   double pos_y, double speed_x, double speed_y,
                                   std::string name, QColor color) {
    assert(mass > 0.);
    return this->add(
        new Astre(
            mass, radius,
            pos_x, pos_y,
            speed_x, speed_y,
            name, color
        )
    );
}



/**
 * Remove given interactant from engine, without call its destructor.
 * Return true if something have been removed.
 */
bool eng::Engine::remove(eng::Interactant * const target, bool delete_it) {
    if(delete_it) {
        uint64_t old_len = astres.size();
        astres.remove(target);
        return astres.size() != old_len;
    } else {  // don't delete it
        bool found = false;
        std::list<Interactant*> new_list;
        for(auto elem : this->astres) {
            if(elem != target) {
                new_list.push_back(elem);
                found = true;
            }
        }
        this->astres = new_list;
        return found;
    }
}


/**
 * Spawn given system with given offset in position.
 * Offset is basically the center of the system.
 */
void eng::Engine::spawn(System& system, std::tuple<double, double> offset) {
    this->spawn(system, std::make_tuple(std::get<0>(offset),
                                        std::get<1>(offset), 0., 0.));
}
void eng::Engine::spawn(System& system, PositionAndSpeed offset) {
    PositionAndSpeed astre_pos = system.position();
    double pos_x = std::get<0>(offset) + unit::meter_to_au(std::get<0>(astre_pos));
    double pos_y = std::get<1>(offset) + unit::meter_to_au(std::get<1>(astre_pos));
    double spd_x = std::get<2>(offset) + std::get<2>(astre_pos);
    double spd_y = std::get<3>(offset) + std::get<3>(astre_pos);
    PositionAndSpeed astre_absolute_pos(std::make_tuple(pos_x, pos_y, spd_x, spd_y));
    this->add(system.getAstre(), astre_absolute_pos);
    for(auto subsystem : system.getSubsystems()) {
        this->spawn(*subsystem, astre_absolute_pos);
    }
}


void eng::Engine::update() {
    //std::cout << "engine updated !" << std::endl;
    auto it_astres = this->astres.begin();
    auto it_interactor = this->astres.begin();
    // make the astre consider themselves, and handle collisions
    for(; it_astres != this->astres.end(); it_astres++) {
        it_interactor = it_astres;
        it_interactor++;  // don't perform interactions between same astres
        for(; it_interactor != this->astres.end(); it_interactor++) {
            Interactant* astre = *it_astres;
            Interactant* other = *it_interactor;
            double dist = astre->distTo(other);
            assert(astre != other);
            if(astre->updatable() and other->updatable()) {
#if COLLISION
                if(astre->collide(other)) {
                    //std::cout << "Collision detected !" << std::endl;
                    // the bigger absorbs the ligher
                    if(astre->bigger(other)) {
                        astre->absorbs(other);
                    } else {
                        other->absorbs(astre);
                    }
                } else {
#endif
                    astre->accelerateTo(other, dist);
                    other->accelerateTo(astre, dist);
#if COLLISION
                } // astre in collision or totally used
#endif
            } // astre not updatable
        }
    }
    // update all astres
    it_astres = this->astres.begin();
    for(; it_astres != this->astres.end(); it_astres++) {
        Interactant* astre = *it_astres;
        if(astre->updatable()) {
            astre->physic_update();
        } else {  // astre was destroyed, removed,…
            delete astre;
            it_astres = this->astres.erase(it_astres);
        }
    }
    // debug
    //std::cout << "Interactant count: " << this->astres.size() << std::endl;
}


void eng::Engine::clear_all() {
    // first, nullify them all for avoid any side effect on concurrent
    //  call on the list.
    auto it_astres = this->astres.begin();
    for(; it_astres != this->astres.end(); it_astres++) {
        (*it_astres)->nullify();
    }
    // then free then all.
    it_astres = this->astres.begin();
    for(; it_astres != this->astres.end(); it_astres++) {
        delete *it_astres;
        it_astres = this->astres.erase(it_astres);
    }
}
