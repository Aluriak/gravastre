#include <QApplication>
#include <iostream>
#include "simulationFactory.h"
#include "engine.h"
#include "view.h"
#include "system.h"

// random number generator intialization
#include <random>


int main(int argc, char *argv[]) {
    QApplication qapp(argc, argv);

    eng::SimulationFactory::InitMode mode;
    bool start_paused = false;
    bool do_solarsystem = true;
    std::string json_file;

    if(argc > 1) {
        mode = eng::SimulationFactory::InitMode::ReadJson;
        json_file = std::string(argv[1]);
        if(argc > 2) {
            start_paused = std::string("--paused") == std::string(argv[2]);
        }
    } else if(do_solarsystem) {
        // init solar system example
        mode = eng::SimulationFactory::InitMode::BuiltinSystem;
    } else {
        // random distribution of asteroïds
        mode = eng::SimulationFactory::InitMode::BuiltinRandom;
    }

    LOGOK
    //eng::SimulationFactory factory(json_file, start_paused, mode);
    eng::SimulationFactory factory("../data/player.json", start_paused, eng::SimulationFactory::InitMode::ReadJson);
    factory.build();

    return qapp.exec();
}
