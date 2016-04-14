#include <QApplication>
#include <iostream>
#include "engine.h"
#include "view.h"
#include "system.h"

// random number generator intialization
#include <random>


int main(int argc, char *argv[]) {
    QApplication qapp(argc, argv);

    eng::Engine engine;

    bool do_solarsystem = true;
    //do_solarsystem = false;
    if(argc > 1) {
        eng::System* system = eng::System::from_json(argv[1])[0];
        engine.spawn(*system);
    } else if(do_solarsystem) {
        // init solar system example
        //                   mass,   X,   Y, speedX, speedY,      name, color
        engine.add_astre(   2e30,    1,   1,      0,      0,     "sun", Qt::yellow);
        engine.add_astre( 3.3e23, 1.38,   1,      0,  47.15, "mercure", Qt::darkGray);
        engine.add_astre(4.87e24, 1.72,   1,      0,  35.15,   "venus", Qt::darkYellow);
        engine.add_astre(5.98e24,    2,   1,      0,  29.82,   "earth", Qt::blue);
        engine.add_astre(7.34e22,2.00257, 1,      0,  30.82,    "moon", Qt::white);
        engine.add_astre(5.98e24,  2.5,   1,      0,  24.35,    "mars", Qt::red);
        std::cout << "KILOMETER_PER_PIXEL = " << KILOMETER_PER_PIXEL << std::endl;
        std::cout << "PIXEL_PER_KILOMETER = " << PIXEL_PER_KILOMETER << std::endl;
        std::cout << "   KILOMETER_PER_AU = " << KILOMETER_PER_AU << std::endl;
        std::cout << "    METER_PER_PIXEL = " << METER_PER_PIXEL << std::endl;
        std::cout << "    PIXEL_PER_METER = " << PIXEL_PER_METER << std::endl;
    } else {
        std::random_device random_rd;     // only used once to initialise (seed) engine
        std::mt19937 random_gen(random_rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> distribution_pos(0,200);
        std::uniform_int_distribution<int> distribution_spd(-3,3);
        std::uniform_int_distribution<int> distribution_mass(10,18);
        for(int i = 0; i < 1500; i++) {
            double pos_x = distribution_pos(random_gen) / 100.;
            double pos_y = distribution_pos(random_gen) / 100.;
            int spd_x = distribution_spd(random_gen);
            int spd_y = distribution_spd(random_gen);
            engine.add_astre(pow(10, distribution_mass(random_gen)), pos_x, pos_y, spd_x, spd_y, "unamed");
        }
    }

    view::Universe universe_view(engine);
    universe_view.show();

    return qapp.exec();
}
