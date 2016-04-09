#include "system.h"





eng::System::System(Astre* astre, std::vector<System*> subsystems, double distance) :
            astre(astre), subsystems(subsystems), distance(distance) {}


/**
 * Parse file of given filename as a JSON formatted file,
 * seeking for Astre definitions.
 * Return a new System, spawning Astre found by parsing.
 */
std::vector<eng::System*> eng::System::from_json(std::string filename) {
    return System::from_json(utils::json_from_file(filename));
}
std::vector<eng::System*> eng::System::from_json(QJsonArray json) {
    std::vector<eng::System*> systems;
    for(auto value : json) {
        assert(value.isObject());
        QJsonObject object = value.toObject();
        System::validate(object);
        double distance = object[SYSTEM_JSON_KEY_DISTANCE].isNull() ?
                          0. : object[SYSTEM_JSON_KEY_DISTANCE].toDouble();
        // Create the new System representing object
        systems.push_back(new System(
            System::json_to_astre(object),
            System::from_json(object[SYSTEM_JSON_KEY_SATELLITES].toArray()),
            distance
        ));
    }
    return systems;
}

eng::Astre* eng::System::json_to_astre(QJsonObject json) {
    double speed_x, speed_y = 0.;
    speed_x = json[SYSTEM_JSON_KEY_SPEED].toArray()[0].toDouble();
    speed_y = json[SYSTEM_JSON_KEY_SPEED].toArray()[1].toDouble();
    double mass = json[SYSTEM_JSON_KEY_MASS].toDouble();
    assert(json[SYSTEM_JSON_KEY_COLOR].isArray());
    return new Astre(
        mass, Astre::mass_to_radius(mass),
        json[SYSTEM_JSON_KEY_DISTANCE].toDouble(), 0.,
        unit::kilometer_to_meter(speed_x),  // km.s-1 to m.s-1
        unit::kilometer_to_meter(speed_y),  // (Astre expects m.s-1)
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
#if DEBUG_JSON_PARSING_LOGS
        std::cerr << "System_validate_logging: QJsonValue '"
            << value.toString().toStdString() << "' profile: "
            << value.isArray() << value.isBool() << value.isDouble()
            << value.isNull() << value.isObject() << value.isString()
            << value.isUndefined() << std::endl;
#endif
}}
void eng::System::validate(QJsonObject system) {
    if(not system[SYSTEM_JSON_KEY_NAME].isNull()) {
        System_validate_logging(system[SYSTEM_JSON_KEY_NAME]);
        assert(system[SYSTEM_JSON_KEY_NAME].isString());
    }
    if(not system[SYSTEM_JSON_KEY_MASS].isNull()) {
        System_validate_logging(system[SYSTEM_JSON_KEY_MASS]);
        assert(system[SYSTEM_JSON_KEY_MASS].isDouble());
    }
    if(not system[SYSTEM_JSON_KEY_COLOR].isNull()) {
        System_validate_logging(system[SYSTEM_JSON_KEY_COLOR]);
        assert(system[SYSTEM_JSON_KEY_COLOR].isArray());
    }
    if(not system[SYSTEM_JSON_KEY_SPEED].isNull()) {
        System_validate_logging(system[SYSTEM_JSON_KEY_SPEED]);
        assert(system[SYSTEM_JSON_KEY_SPEED].isArray());
    }
    if(not system[SYSTEM_JSON_KEY_SATELLITES].isNull()) {
        System_validate_logging(system[SYSTEM_JSON_KEY_SATELLITES]);
        assert(system[SYSTEM_JSON_KEY_SATELLITES].isArray());
    }
    if(not system[SYSTEM_JSON_KEY_DISTANCE].isNull()) {
        System_validate_logging(system[SYSTEM_JSON_KEY_DISTANCE]);
        assert(system[SYSTEM_JSON_KEY_DISTANCE].isDouble());
    }
}
