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
}




/**
 * Validate document.
 */
void utils::JsonConfig::validateDocument() {
    assert(this->doc.isObject());
    assert(not this->doc.isArray());

    // validates options
    for(auto value : this->doc.object()[SYSTEM_JSON_ROOT_OPTIONS].toObject()) {
        assert(value.isObject());
    }

    // validates players
    for(auto value : this->doc.object()[SYSTEM_JSON_ROOT_PLAYERS].toObject()) {
        assert(value.isObject());
    }

    // validates systems
    for(auto value : this->doc.object()[SYSTEM_JSON_ROOT_SYSTEMS].toObject()) {
        assert(value.isObject());
    }
}



