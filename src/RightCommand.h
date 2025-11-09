#pragma once
#include "Command.h"
#include "Entity.h"

class RightCommand : public Command {
	Entity* m_entity;
	float   m_acc;
	float   m_dec;
	float   m_airAcc;
	float   m_maxSpeed;

public:
	RightCommand(
		Entity* entity,
		float   acc,
		float   dec,
		float   airAcc,
		float   maxSpeed
	);

	void execute();
};