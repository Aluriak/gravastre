#pragma once

#include <iostream>
#include "options.h"


// Debug flag
#define LOGOK std::cerr << __FILE__ << ':' << __LINE__ << '\n';


/**
 * Next are defined constants that are immutable, relative to
 * physical definitions, or deduced from previously defined constants.
 */
#define PI (double)3.141592
#define PI_DIVIDE_180 (double)57.296
// conversion units
#define METER_PER_KILOMETER 1e3
#define KILOMETER_PER_METER 1./METER_PER_KILOMETER
// graviationnal constant. The one.
// NB: The simulation speed has a direct impact on it.
//     The speedy the simulation, the higher should be G
//     for keep the mathematic equations cohesion.
#define PHYSIC_GRAVITATIONNAL_CONSTANT 6.67384e-11
#define GRAVITATIONNAL_CONSTANT PHYSIC_GRAVITATIONNAL_CONSTANT*SIMULATION_SPEED
// AU: Astronomic Unit, about 150 000 000 kilometers
#define METER_PER_AU 150e9
#define AU_PER_METER 1./METER_PER_AU
#define KILOMETER_PER_AU METER_PER_AU/METER_PER_KILOMETER
#define AU_PER_KILOMETER METER_PER_KILOMETER/METER_PER_AU
// deduce the km.px-1 and px.km-1 values
#define KILOMETER_PER_PIXEL KILOMETER_PER_AU/PIXEL_PER_AU
#define PIXEL_PER_KILOMETER PIXEL_PER_AU/KILOMETER_PER_AU
// deduce the m.px-1 and px.m-1 values
#define METER_PER_PIXEL METER_PER_AU/PIXEL_PER_AU
#define PIXEL_PER_METER PIXEL_PER_AU/METER_PER_AU


/**
 * Contains declarations of functions allowing units conversions,
 * plus a function estimating the attraction force according to
 * the well know graviationnal attraction equation.
 */
namespace unit {

    inline double meter_to_au(double meter) {
        return meter * AU_PER_METER;
    }
    inline double au_to_meter(double au) {
        return au * METER_PER_AU;
    }

    inline double kilometer_to_au(double km) {
        return km * AU_PER_KILOMETER;
    }
    inline double kilometer_to_meter(double km) {
        return km * METER_PER_KILOMETER;
    }
    inline double meter_to_kilometer(double km) {
        return km * KILOMETER_PER_METER;
    }

    inline double pixel_to_au(double px) {
        return px / PIXEL_PER_AU;
    }
    inline double au_to_pixel(double au) {
        return au * PIXEL_PER_AU;
    }

    inline double pixel_to_meter(double px) {
        return (px / PIXEL_PER_AU) * METER_PER_AU;
    }
    inline double meter_to_pixel(double meter) {
        return (meter / METER_PER_AU) * PIXEL_PER_AU;
    }
    inline double kilometer_to_pixel(double km) {
        return (km / KILOMETER_PER_AU) * PIXEL_PER_AU;
    }

    inline double kilometer_per_second_to_au_per_second(double km_per_sec) {
        return km_per_sec * AU_PER_KILOMETER;
    }
    inline double au_per_second_to_kilometer_per_second(double au_per_sec) {
        return au_per_sec * KILOMETER_PER_AU;
    }

    inline double real_speed_to_simulation_speed(double speed) {
        return speed * SIMULATION_SPEED;
    }
    inline double simulation_speed_to_real_speed(double speed) {
        return speed / SIMULATION_SPEED;
    }

    inline double degree_to_radian(double degree) {
        return degree / PI_DIVIDE_180;
    }
    inline double radian_to_degree(double radian) {
        return radian * PI_DIVIDE_180;
    }

    /**
     * Return the Newton force value caused by graviationnal attraction between
     * two corpses of masses 1 and 2 (kg), distant from each other
     * to given distance in meter.
     */
    inline double attraction_force_meter(double mass1, double mass2, double meter_dist) {
        return - GRAVITATIONNAL_CONSTANT * mass1 * mass2 / (meter_dist*meter_dist);
    }

} // namespace unit

