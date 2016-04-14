#pragma once

#include <iostream>
#include <assert.h>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>

#include "astre.h"
#include "utils.h"
#include "orbit.h"


namespace eng {

    /**
     * Factory of Astre, and a Composite.
     */
    class System {

    public:
        System();
        System(AstreData, orbit::OrbitalTrajectory, std::vector<System*>);
        static std::vector<eng::System*> from_json(std::string);
        static std::vector<eng::System*> from_json(QJsonArray, double* =NULL);
        static AstreData json_to_astre(QJsonObject);
        static void validate(QJsonObject system);

        AstreData getAstre() const { return this->astre; }
        orbit::OrbitalTrajectory getTrajectory() { return this->trajectory; }
        //double getMass()      const { return std::get<0>(this->astre); }
        //double getRadius()    const { return std::get<1>(this->astre); }
        //std::string getName() const { return std::get<2>(this->astre); }
        //QColor getColor()     const { return std::get<3>(this->astre); }

        // Accessors
        std::vector<System*> getSubsystems() const { return this->subsystems; }
        std::tuple<double, double, double, double> position() const
                        { return this->trajectory.position(); }


    private:
        AstreData astre;
        orbit::OrbitalTrajectory trajectory;
        std::vector<System*> subsystems;

    };

} // namespace view
