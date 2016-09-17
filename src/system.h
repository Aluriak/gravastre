#pragma once

#include <iostream>
#include <cassert>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "astre.h"
#include "utils.h"
#include "orbit.h"


namespace eng {


    /**
     * System data as tuple of three values:
     *  AstreData, trajectory, subsystems
     */
    class System;
    typedef std::tuple<AstreData, orbit::OrbitalTrajectory,
                       std::vector<System*>> SystemData;


    /**
     * Factory of Astre, and a Composite.
     */
    class System {

    public:
        //System();
        System(SystemData);
        static std::vector<System*> from(utils::JsonConfig&);
        static std::vector<System*> from(QJsonArray, double* =NULL);
        static AstreData json_to_astre(QJsonObject);
        static void validate(QJsonObject system);

        // Accessors
        AstreData getAstre() const { return this->astre; }
        orbit::OrbitalTrajectory getTrajectory() const { return this->trajectory; }
        double getDiameter() const { return this->diameter; }
        //double getMass()      const { return std::get<0>(this->astre); }
        //double getRadius()    const { return std::get<1>(this->astre); }
        //std::string getName() const { return std::get<2>(this->astre); }
        //QColor getColor()     const { return std::get<3>(this->astre); }
        std::vector<System*> getSubsystems() const { return this->subsystems; }
        std::tuple<double, double, double, double> position() const
                        { return this->getTrajectory().position(); }


    private:
        // Methods
        void computeDiameter();

        // Attributes
        AstreData astre;
        orbit::OrbitalTrajectory trajectory;
        std::vector<System*> subsystems;
        double diameter; // approximative maximal size in AU

    };

} // namespace view
