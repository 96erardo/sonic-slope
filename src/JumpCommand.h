#pragma once
#include "Command.h"
#include "Entity.h"

class JumpCommand : public Command {
	Entity* m_entity;
	double  m_jumpSpeed;
public:
	JumpCommand(Entity* entity, double jumpSpeed);

	void execute();
};