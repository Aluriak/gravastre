#pragma once


/**
 * Next are defined constants that depends of the expected simulation behavior
 */
#define PIXEL_PER_AU 300  // number of pixel representing an AU
#define SIMULATION_SPEED 8e3  // how speedy is the simulation, compared to reality
#define COLLISION true  // set to false for avoid collisions


/**
 * Next are defined some debug flags, enabling some debug behaviors,
 * useful for debugging or logging.
 * Note that some feature may need activation of previously defined options.
 */
// show distance to the Astre named "sun", maximal, minimal and diff.
// can be used for show modifications of orbits.
#define DEBUG_SUN_DIST false
#define DEBUG_CREATION_LOGS false
#define DEBUG_COLLISION_LOGS false
#define DEBUG_DISTANCES_LOGS false
#define DEBUG_NULLIFIED_LOGS false
#define DEBUG_ABSORBED_LOGS false
#define DEBUG_ORBITAL_TRAJECTORY_LOGS false


/**
 * Next are defined some Interactant related flags, enabling some particular data manipulation,
 * useful for more features.
 * Note that some feature may need activation of previously defined options.
 */
#define DATA_INTERACTANTS_HOLDS_TRAJECTORY false


/**
 * Next are defined some view flags, enabling some special view options,
 * useful for more playing features.
 * Note that some feature may need activation of previously defined options.
 */
#define VIEW_ORBITAL_TRAJECTORY DATA_ASTRE_HOLDS_TRAJECTORY && false
#define VIEW_INITIAL_POSITION false
#define VIEW_DETAILS_ON_MOUSE_CLIC true
#define VIEW_REVERSED_TRANSLATION false
#define VIEW_TRANSLATION_RATIO 1.0
#define VIEW_VISIBLE_REFERENCE false
#define VIEW_SELECTABLE_REFERENCE false  // true: some bugs can appear
#define VIEW_ENGINE_TIME_UPDATE 5  // milliseconds
#define DEFAULT_RADIUS_BY_MASS_FACTOR 1e23  // factor used to compute a default radius from mass
#define RADIUS_TO_PIXEL_FACTOR 1e7  // factor used to compute radius in pixel from radius in km
#define ALWAYS_USE_DEFAULT_RADIUS false  // always use default radius, even if it is provided
