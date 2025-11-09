#include <cmath>
#include "RightCommand.h"

RightCommand::RightCommand(
    Entity* entity,
    float   acc,
    float   dec,
    float   airAcc,
    float   maxSpeed
) : m_entity(entity), m_acc(acc), m_dec(dec), m_airAcc(airAcc), m_maxSpeed(maxSpeed)
{
}

void RightCommand::execute()
{
    m_entity->getComponent<CInput>().right = true;

    std::cout << "RIGHT" << std::endl;

    if (m_entity->getComponent<CGroundSpeed>().grounded) {
        m_entity->getComponent<CState>().state = "Running";

        if (m_entity->getComponent<CGroundSpeed>().speed < 0) {
            m_entity->getComponent<CGroundSpeed>().speed = std::min(
                m_maxSpeed,
                m_entity->getComponent<CGroundSpeed>().speed + m_dec
            );

            m_entity->getComponent<CState>().state = "Stopping";

        }
        else {
            m_entity->getComponent<CGroundSpeed>().speed = std::min(
                m_maxSpeed,
                m_entity->getComponent<CGroundSpeed>().speed + m_acc
            );

            m_entity->getComponent<CTransform>().scale = Vec2(1, 1);

            if (abs(m_entity->getComponent<CGroundSpeed>().speed) > (m_maxSpeed / 2)) {
                m_entity->getComponent<CState>().state = "RunningFast";
            }
        }
    }
    else {
        m_entity->getComponent<CTransform>().vel.x = std::min(
            m_maxSpeed,
            m_entity->getComponent<CTransform>().vel.x + m_airAcc
        );

        m_entity->getComponent<CTransform>().scale = Vec2(1, 1);

        if (m_entity->getComponent<CState>().state != "Jumping") {
            if (abs(m_entity->getComponent<CTransform>().vel.x) > (m_maxSpeed / 2)) {
                m_entity->getComponent<CState>().state = "RunningFast";
            }
        }
    }
}
