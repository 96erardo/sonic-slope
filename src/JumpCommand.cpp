#define _USE_MATH_DEFINES
#include <cmath>
#include "JumpCommand.h"

JumpCommand::JumpCommand(Entity* entity, double jumpSpeed): m_entity(entity), m_jumpSpeed(jumpSpeed)
{
}

void JumpCommand::execute() {
    m_entity->getComponent<CInput>().jump = true;

	if (m_entity->getComponent<CGroundSpeed>().grounded) {
        m_entity->getComponent<CState>().state = "Jumping";
        m_entity->getComponent<CGroundSpeed>().grounded = false;
        m_entity->getComponent<CTransform>().vel.x -= m_jumpSpeed * sinf((360 - m_entity->getComponent<CTransform>().angle) * (M_PI / 180));
        m_entity->getComponent<CTransform>().vel.y -= m_jumpSpeed * cosf((360 - m_entity->getComponent<CTransform>().angle) * (M_PI / 180));
        m_entity->getComponent<CTransform>().angle = 0;
	}
}
