#pragma once


#include <iostream>
#include <tuple>
#include <random>  // random number generator intialization

#include "jsonConfig.h"
#include "view.h"
#include "engine.h"
#include "astre.h"
#include "player.h"
#include "system.h"



namespace eng {

    class SimulationFactory {


    public:
        // Nested
        enum InitMode {ReadJson, BuiltinSystem, BuiltinRandom};

        // Lifetime
        SimulationFactory(std::string json_file, bool start_paused=false, SimulationFactory::InitMode mode=ReadJson);

        // Methods
        view::Universe* build() const;

    protected:
        void generateRandomUniverse(Engine& engine) const;
        void generateBuiltinSolarSystem(Engine& engine) const;
        void generateFromJSON(Engine& engine) const;

    private:
        std::string json_file;
        bool start_paused;
        SimulationFactory::InitMode mode;
        utils::JsonConfig* config = nullptr;

    };

} // namespace eng
