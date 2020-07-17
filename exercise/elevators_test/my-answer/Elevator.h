#pragma once

#include "Messages.h"
#include "Utils.h"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

enum Direction;
enum DirectionEx;
enum ShaftStop { On=0, Off };

class Shaft {
public:
	ShaftStop up;
	ShaftStop down;
};

class MessageElevatorCall;

class Elevator
{
public:
											Elevator(
												const unsigned int		aId,
												const unsigned int		aFloorCount);

											Elevator(
												const unsigned int		aId,
												const unsigned int		aFloorCount,
												const unsigned int		aCurrentFloor,
												const Direction			aDirection);

	void									SelectFloor(
												const unsigned int		aFloorId);

	unsigned int							CurrentFloor() const;
	Direction								CurrentDirection() const;
	bool									HasWork() const;

	std::vector<ShaftStop>					GetAllStates(
												const std::vector<ShaftStop>& statesInside,
												const std::vector<Shaft>&statesOnside) const;

	void									Step();
	DirectionEx								TryArrive();
	unsigned int							Id() const;

	std::string								ToString() const;

	float                                   getHardness(
												unsigned int			floor,
												Direction				directione) const;

	void									CleanStatesOutside();
	void									getCallCommand(
												unsigned int			calledFloor, 
												Direction				direction);

private:
	unsigned int							myId;
	unsigned int							myFloorCount;
	unsigned int							myCurrentFloor;
	Direction								myCurrentDirection;
	unsigned int							LowestActiveFloor(
												std::vector<ShaftStop>& targetStates) const ;

	unsigned int							HighestActiveFloor(
												std::vector<ShaftStop>& targetStates) const ;

	unsigned int							ActiveFloorCount(
												std::vector<ShaftStop>& targetStates) const;
	unsigned int							LowerActiveFloorCount(
												std::vector<ShaftStop>& targetStates, unsigned int floor) const;
	unsigned int							HighterActiveFloorCount(
												std::vector<ShaftStop>& targetStates, unsigned intfloor) const;

	std::vector<ShaftStop> myStatesInside;
	std::vector<Shaft> myStatesOutside;
};
