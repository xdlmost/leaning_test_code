#pragma once

#include <string>
#include "Elevator.h"

enum Direction
{
	Up = 0,
	Down,
};
enum DirectionEx 
{ 
	exUp=0, 
	exDown, 
	exNoWay, 
	exTwoWay,
	exStop
};

class MessageElevatorReady
{
};

class MessageElevatorStep
{
};

class MessageElevatorCall
{
public:
	unsigned int			myFloor;
	Direction				myDirection;
};

class MessageElevatorRequest
{
public:
	unsigned int			myElevatorId;
	unsigned int			myFloor;
};

class MessageElevatorArrived
{
public:
	unsigned int			myElevatorId;
	unsigned int			myFloor;
	DirectionEx				myDirectionEx;
};

class MessageHumanStep
{
};

class MessageHumanDone
{
};

static std::string
ToString(
	const Direction		aDirection)
{
	switch (aDirection)
	{
	case Direction::Up:
		return "Up";
	case Direction::Down:
		return "Down";
	default:
		return "Unknown";
		break;
	}
}