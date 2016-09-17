#include "jsonConfig.h"


/**
 * Lifetime
 */
utils::JsonConfig::JsonConfig()  { }


utils::JsonConfig::JsonConfig(QJsonDocument doc) : doc(doc) {
    this->validateDocument();
    this->parseDocument();
}

utils::JsonConfig::JsonConfig(std::string filename) {
    QFile file(filename.c_str());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "File " << filename << " not opened." << std::endl;
        exit(1);
    }
    this->doc = QJsonDocument::fromJson(file.readAll());
    // init
    this->validateDocument();
    this->parseDocument();
}


utils::JsonConfig::~JsonConfig() {
    for(auto player : this->players) {
        delete player;
    }
}





/**
 * Parse document in order to retrieve all available data.
 */
void utils::JsonConfig::parseDocument() {
    LOGOK
    if(this->doc.isArray()) {  // systems are provided directly
    LOGOK
        // get systems
        systems = this->doc.array();
    } else {  // full form
    LOGOK
        assert(this->doc.isObject());
        assert(not this->doc.isArray());

        // get options
        options = this->doc.object()[SYSTEM_JSON_ROOT_OPTIONS].toObject();

        // get players
        auto player_objects = this->doc.object()[SYSTEM_JSON_ROOT_PLAYERS].toObject().toVariantMap();
        for(QString player_name : player_objects.keys()) {
            this->players.push_back(new Player(std::string(player_name.toStdString())));
        }

        // get systems
        systems = this->doc.object()[SYSTEM_JSON_ROOT_SYSTEMS].toArray();
    LOGOK
    }
    LOGOK
}




/**
 * Validate document.
 */
void utils::JsonConfig::validateDocument() {
    if(this->doc.isArray()) {  // systems are provided directly
        // validates systems
        for(auto value : this->doc.array()) {
            assert(value.isObject());
        }
    } else {  // full form
        assert(this->doc.isObject());
        assert(not this->doc.isArray());
        auto field_options = this->doc.object()[SYSTEM_JSON_ROOT_OPTIONS];
        auto field_players = this->doc.object()[SYSTEM_JSON_ROOT_PLAYERS];
        auto field_systems = this->doc.object()[SYSTEM_JSON_ROOT_SYSTEMS];
        // validates options
        if(not field_options.isNull()) {
            for(auto value : field_options.toObject()) {
                assert(value.isObject());
            }
        }
        // validates players
        if(not field_players.isNull()) {
            for(auto value : field_players.toObject()) {
                assert(value.isObject());
            }
        }
        // validates systems
        if(not field_systems.isNull()) {
            for(auto value : field_systems.toObject()) {
                assert(value.isObject());
            }
        }
    }
}



