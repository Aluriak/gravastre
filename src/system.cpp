#include "system.h"


eng::System::System() {}


eng::System::System(AstreData astre, orbit::OrbitalTrajectory trajectory,
                    std::vector<System*> subsystems) :
                        astre(astre), trajectory(trajectory),
                        subsystems(subsystems) {}


/**
 * Parse file of given filename as a JSON formatted file,
 * seeking for Astre definitions.
 * Return a new System, spawning Astre found by parsing.
 */
std::vector<eng::System*> eng::System::from_json(std::string filename) {
    return System::from_json(utils::json_from_file(filename));
}
std::vector<eng::System*> eng::System::from_json(QJsonArray json, double* parent_mass) {
    std::vector<eng::System*> systems;
    for(auto value : json) {
        assert(value.isObject());
        QJsonObject object = value.toObject();
        System::validate(object);
        orbit::OrbitalTrajectory trajectory = orbit::OrbitalTrajectory(object,
                                                                       parent_mass);
#if DEBUG_ORBITAL_TRAJECTORY_LOGS
        trajectory.debug();
#endif
        if(parent_mass != NULL and not trajectory.is_orbit()) {
            // Logs the problem if should be orbiting something
            std::cerr << "The System of name " << object[SYSTEM_JSON_KEY_NAME].toString().toStdString() << " doesn't have enough data to be placed in orbit. It will not be created." << std::endl;
        } else { // no parent OR orbit found
            // Create the new System representing object
            AstreData astre(System::json_to_astre(object));
            systems.push_back(new System(
                astre,
                trajectory,
                System::from_json(
                    object[SYSTEM_JSON_KEY_SATELLITES].toArray(),
                    &std::get<0>(astre)
                )
            ));
        }
    }
    return systems;
}

eng::AstreData eng::System::json_to_astre(QJsonObject json) {
    return AstreData(
        json[SYSTEM_JSON_KEY_MASS].toDouble(),
        json[SYSTEM_JSON_KEY_RADIUS].isNull()?
            -1. : json[SYSTEM_JSON_KEY_RADIUS].toDouble(),
        json[SYSTEM_JSON_KEY_NAME].toString().toStdString(),
        QColor(json[SYSTEM_JSON_KEY_COLOR].toArray()[0].toInt(),
               json[SYSTEM_JSON_KEY_COLOR].toArray()[1].toInt(),
               json[SYSTEM_JSON_KEY_COLOR].toArray()[2].toInt())
    );
}





/**
 * Data validation: validate that a JSON object is a valid system
 */
namespace eng {inline void System_validate_logging(QJsonValue value) {
        std::cerr << "System_validate_logging: QJsonValue '"
            << value.toString().toStdString() << "' profile: "
            << value.isArray() << value.isBool() << value.isDouble()
            << value.isNull() << value.isObject() << value.isString()
            << value.isUndefined() << std::endl;
}}
void eng::System::validate(QJsonObject system) {
    if(not system[SYSTEM_JSON_KEY_NAME].isNull()) {
        if(not system[SYSTEM_JSON_KEY_NAME].isString()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_NAME]);
            assert(system[SYSTEM_JSON_KEY_NAME].isString());
        }
    }
    if(not system[SYSTEM_JSON_KEY_MASS].isNull()) {
        if(not system[SYSTEM_JSON_KEY_MASS].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_MASS]);
            assert(system[SYSTEM_JSON_KEY_MASS].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_RADIUS].isNull()) {
        if(not system[SYSTEM_JSON_KEY_RADIUS].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_RADIUS]);
            assert(system[SYSTEM_JSON_KEY_RADIUS].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_COLOR].isNull()) {
        if(not system[SYSTEM_JSON_KEY_COLOR].isArray()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_COLOR]);
            assert(system[SYSTEM_JSON_KEY_COLOR].isArray());
        }
    }
    if(not system[SYSTEM_JSON_KEY_SPEED].isNull()) {
        if(not system[SYSTEM_JSON_KEY_SPEED].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_SPEED]);
            assert(system[SYSTEM_JSON_KEY_SPEED].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_SATELLITES].isNull()) {
        if(not system[SYSTEM_JSON_KEY_SATELLITES].isArray()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_SATELLITES]);
            assert(system[SYSTEM_JSON_KEY_SATELLITES].isArray());
        }
    }
    if(not system[SYSTEM_JSON_KEY_DISTANCE].isNull()) {
        if(not system[SYSTEM_JSON_KEY_DISTANCE].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_DISTANCE]);
            assert(system[SYSTEM_JSON_KEY_DISTANCE].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_APOAPSIS].isNull()) {
        if(not system[SYSTEM_JSON_KEY_APOAPSIS].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_APOAPSIS]);
            assert(system[SYSTEM_JSON_KEY_APOAPSIS].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_PERIAPSIS].isNull()) {
        if(not system[SYSTEM_JSON_KEY_PERIAPSIS].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_PERIAPSIS]);
            assert(system[SYSTEM_JSON_KEY_PERIAPSIS].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_ECCENTRICITY].isNull()) {
        if(not system[SYSTEM_JSON_KEY_ECCENTRICITY].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_ECCENTRICITY]);
            assert(system[SYSTEM_JSON_KEY_ECCENTRICITY].isDouble());
        } else {
            double eccentricity = system[SYSTEM_JSON_KEY_ECCENTRICITY].toDouble();
            assert(0 < eccentricity and eccentricity < 1.);
        }
    }
    if(not system[SYSTEM_JSON_KEY_SEMIMAJORAXIS].isNull()) {
        if(not system[SYSTEM_JSON_KEY_SEMIMAJORAXIS].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_SEMIMAJORAXIS]);
            assert(system[SYSTEM_JSON_KEY_SEMIMAJORAXIS].isDouble());
        }
    }
    if(not system[SYSTEM_JSON_KEY_SEMIMINORAXIS].isNull()) {
        if(not system[SYSTEM_JSON_KEY_SEMIMINORAXIS].isDouble()) {
            System_validate_logging(system[SYSTEM_JSON_KEY_SEMIMINORAXIS]);
            assert(system[SYSTEM_JSON_KEY_SEMIMINORAXIS].isDouble());
        }
    }
}
