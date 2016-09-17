#include "simulationFactory.h"


/**
 * Lifetime
 */
eng::SimulationFactory::SimulationFactory(std::string json_file, bool start_paused,
                                          eng::SimulationFactory::InitMode mode) :
        json_file(json_file), start_paused(start_paused), mode(mode)
{
    if(this->mode == eng::SimulationFactory::InitMode::ReadJson) {
        this->config = new utils::JsonConfig(this->json_file);
        std::cout << "found player: " << this->config->getPlayers().size() << std::endl;
        std::cout << "found system: " << this->config->getSystems().size() << std::endl;
    } else {
        this->config = new utils::JsonConfig();
    }
}


/**
 * Return a fresh view on a fully initialized universe
 */
view::Universe* eng::SimulationFactory::build() const {
    // Setup engine
    Engine* engine = new Engine();
    if(mode == eng::SimulationFactory::InitMode::ReadJson) {
        LOGOK
        this->generateFromJSON(*engine);
    } else if(mode == eng::SimulationFactory::InitMode::BuiltinSystem) {
        LOGOK
        this->generateBuiltinSolarSystem(*engine);
    } else {
        LOGOK
        assert(mode == eng::SimulationFactory::InitMode::BuiltinRandom);
        this->generateRandomUniverse(*engine);
    }

    // launch !
    view::Universe* universe_view = new view::Universe(*engine, *this->config, start_paused);
    universe_view->show();
    return universe_view;
}




void eng::SimulationFactory::generateFromJSON(Engine& engine) const {
    std::vector<eng::System*> systems = eng::System::from(*this->config);
    double decay = 1;  // AU to decay between each spawn
    for(auto system : systems) {
        engine.spawn(*system, std::make_tuple(decay, 1));
        decay += system->getDiameter();
        std::cout << decay << std::endl;
    }
}




void eng::SimulationFactory::generateBuiltinSolarSystem(Engine& engine) const {
    //             mass,   X,   Y, speedX, speedY,      name, color
    engine.add(   2e30,    1,   1,      0,      0,     "sun", Qt::yellow);
    engine.add( 3.3e23, 1.38,   1,      0,  47150, "mercure", Qt::darkGray);
    engine.add(4.87e24, 1.72,   1,      0,  35150,   "venus", Qt::darkYellow);
    engine.add(5.98e24,    2,   1,      0,  29820,   "earth", Qt::blue);
    engine.add(7.34e22,2.00257, 1,      0,  30820,    "moon", Qt::white);
    engine.add(5.98e24,  2.5,   1,      0,  24350,    "mars", Qt::red);
    std::cout << "KILOMETER_PER_PIXEL = " << KILOMETER_PER_PIXEL << std::endl;
    std::cout << "PIXEL_PER_KILOMETER = " << PIXEL_PER_KILOMETER << std::endl;
    std::cout << "   KILOMETER_PER_AU = " << KILOMETER_PER_AU << std::endl;
    std::cout << "    METER_PER_PIXEL = " << METER_PER_PIXEL << std::endl;
    std::cout << "    PIXEL_PER_METER = " << PIXEL_PER_METER << std::endl;
}



void eng::SimulationFactory::generateRandomUniverse(Engine& engine) const {
    std::random_device random_rd;     // only used once to initialise (seed) engine
    std::mt19937 random_gen(random_rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> distribution_pos(0,1000);
    std::uniform_int_distribution<int> distribution_spd(-100,100);
    std::uniform_int_distribution<int> distribution_mass(10,20);
    for(int i = 0; i < 4000; i++) {
        double pos_x = distribution_pos(random_gen) / 100.;
        double pos_y = distribution_pos(random_gen) / 100.;
        int spd_x = distribution_spd(random_gen);
        int spd_y = distribution_spd(random_gen);
        engine.add(pow(10, distribution_mass(random_gen)), pos_x, pos_y, spd_x, spd_y, "unamed");
    }
}
