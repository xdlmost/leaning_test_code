#include "Elevator.h"
#include "MessageBus.h"

#include "Elevators.h"
#include <algorithm>
#define CAP(minVal,maxVal,value) (std::max(std::min((unsigned int)maxVal,value),(unsigned int)minVal))
#define PROBABILITY 0.5

Elevator::Elevator(
	const unsigned int			aId,
	const unsigned int			aFloorCount)
	: myId(aId)
	, myFloorCount(aFloorCount)
	, myCurrentFloor(1)
	, myCurrentDirection(Direction::Up)
{
	for (int i = 0; i < aFloorCount; ++i) 
	{
		myStatesInside.push_back(ShaftStop::Off);
		myStatesOutside.push_back({ ShaftStop::Off ,ShaftStop::Off });
	}
	Log("[Elevator]", myId, "Created", ToString());
}

Elevator::Elevator(
	const unsigned int		aId,
	const unsigned int		aFloorCount,
	const unsigned int		aCurrentFloor,
	const Direction			aDirection)
	: myId(aId)
	, myFloorCount(aFloorCount)
	, myCurrentFloor(aCurrentFloor)
	, myCurrentDirection(aDirection)
{
	for (int i = 0; i < aFloorCount; ++i)
	{
		myStatesInside.push_back(ShaftStop::Off);
		myStatesOutside.push_back({ ShaftStop::Off ,ShaftStop::Off });
	}
	Log("[Elevator]", myId, "Created", ToString());
}

void
Elevator::SelectFloor(
	const unsigned int			aFloorId)
{
	// Implement me!
	myStatesInside[aFloorId - 1] = On;
	Log("@@Elevator::SelectFloor", aFloorId);
}

unsigned int
Elevator::CurrentFloor() const
{
	return myCurrentFloor;
}

Direction
Elevator::CurrentDirection() const
{
	return myCurrentDirection;
}

bool
Elevator::HasWork() const
{
	// Implement me!
	Log("@@Elevator::HasWork()"); 
	
	auto tmpStatse= GetAllStates(myStatesInside,myStatesOutside);
	return ActiveFloorCount(tmpStatse)>0;
}
std::vector<ShaftStop>
Elevator::GetAllStates(const std::vector<ShaftStop>& statesInside, const std::vector<Shaft>&statesOnside) const
{
	std::vector<ShaftStop> states;
	for (int i = 0; i < myFloorCount; ++i)
	{
		states.push_back((ShaftStop)(statesInside[i] & statesOnside[i].up&statesOnside[i].down));
	}
	return states;
}
void
Elevator::Step()
{
	// Implement me!
	if (HasWork())
	{
		auto tmpStatse = GetAllStates(myStatesInside, myStatesOutside);
		if (Direction::Up == myCurrentDirection )
		{
			if ( HighterActiveFloorCount(tmpStatse, myCurrentFloor)>0)
				myCurrentFloor=CAP(1, myFloorCount, myCurrentFloor+1);
			else if (LowerActiveFloorCount(tmpStatse, myCurrentFloor) > 0 )
			{
				myCurrentDirection = Direction::Down;
				myCurrentFloor = CAP(1, myFloorCount, myCurrentFloor - 1);
			}
			if (myCurrentFloor == myFloorCount)
			{
				myCurrentDirection = Direction::Down;
			}

		}
		else if (Direction::Down == myCurrentDirection)
		{
			if (LowerActiveFloorCount(tmpStatse, myCurrentFloor)>0)
				myCurrentFloor = CAP(1, myFloorCount, myCurrentFloor - 1);
			else if (HighterActiveFloorCount(tmpStatse, myCurrentFloor) > 0 )
			{
				myCurrentDirection = Direction::Up;
				myCurrentFloor = CAP(1, myFloorCount, myCurrentFloor + 1);
			}
			if (myCurrentFloor == 1)
			{
				myCurrentDirection = Direction::Up;
			}
		}
		DirectionEx directionEx = TryArrive();
		if (DirectionEx ::exStop!= directionEx)
		{
			MessageElevatorArrived messageElevatorArrived;
			messageElevatorArrived.myDirectionEx = directionEx;
			messageElevatorArrived.myElevatorId = Id();
			messageElevatorArrived.myFloor = myCurrentFloor;
			SEND_TO_HUMANS(messageElevatorArrived);
		}
	}
	else 
	{
		Log("@@Elevator", myId, "Step() at", myCurrentFloor," Work Done!! ");
	}

}

DirectionEx
Elevator::TryArrive()
{
	unsigned int floorIndex = myCurrentFloor - 1;
	auto out = myStatesOutside[floorIndex];
	DirectionEx ret = DirectionEx::exStop;
	if (ShaftStop::On == out.up) 
	{
		if (ShaftStop::On == out.down) 
		{
			ret = DirectionEx::exTwoWay;
		}
		else 
		{
			ret = DirectionEx::exUp;
		}
	}
	else 
	{
		if (ShaftStop::On == out.down)
		{
			ret = DirectionEx::exDown;
		}
		else
		{
			if (ShaftStop::On == myStatesInside[floorIndex]) 
			{
				ret = DirectionEx::exNoWay;
			}
		}
	}
	out.up = ShaftStop::Off;
	out.down = ShaftStop::Off;
	myStatesInside[floorIndex] = ShaftStop::Off;

	Log("@@Elevator", myId, "Step() at ", myCurrentFloor);
	return ret;
}

unsigned int
Elevator::Id() const
{
	return myId;
}

std::string
Elevator::ToString() const
{
	return "- State: " 
		+ std::to_string(myCurrentFloor) 
		+ "/" 
		+ std::to_string(myFloorCount) 
		+ " " 
		+ ::ToString(myCurrentDirection);
}

float
Elevator::getHardness(unsigned int			floor,
					  Direction              direction) const
{
	auto tmpStatseInside(myStatesInside);
	auto tmpStatseOutside(myStatesOutside);
	auto tmpStates=GetAllStates(tmpStatseInside, tmpStatseOutside);
	if (Direction::Up == direction) 
	{
		tmpStatseOutside[floor - 1].up = ShaftStop::On;
	}
	else if (Direction::Up == direction) 
	{
		tmpStatseOutside[floor - 1].down = ShaftStop::On;
	}

	unsigned int activeCount = ActiveFloorCount(tmpStates);
	float hardness_waiting = 0;
	float hardness_traveling = 0;
	unsigned int canbinFloor = myCurrentFloor;
	Direction canbinDirection = myCurrentDirection;

	while (0 != activeCount)
	{
		if (Direction::Up == canbinDirection)
		{
			if (HighterActiveFloorCount(tmpStates, canbinFloor)>0)
				canbinFloor = CAP(1, myFloorCount, canbinFloor + 1);
			else if (LowerActiveFloorCount(tmpStates, canbinFloor) > 0 )
			{
				canbinDirection = Direction::Down;
				canbinFloor = CAP(1, myFloorCount, canbinFloor - 1);
			}
		}
		else if (Direction::Down == canbinDirection)
		{
			if (LowerActiveFloorCount(tmpStates, canbinFloor)>0)
				canbinFloor = CAP(1, myFloorCount, canbinFloor - 1);
			else if (HighterActiveFloorCount(tmpStates, canbinFloor) > 0 )
			{
				canbinDirection = Direction::Up;
				canbinFloor = CAP(1, myFloorCount, canbinFloor + 1);
			}

		}
		{
			auto& out = tmpStatseOutside[canbinFloor - 1];
			auto in = tmpStatseInside[canbinFloor - 1];
			if (ShaftStop::On == in)
			{
				tmpStatseInside[canbinFloor - 1] = ShaftStop::Off;
			}
			if (ShaftStop::On == out.up && (Direction::Up == canbinDirection|| LowerActiveFloorCount(tmpStates, canbinFloor)==0))
			{
				out.up = ShaftStop::Off;
				tmpStatseInside[CAP(1, myFloorCount, (unsigned int)((canbinFloor + myFloorCount + 0.5) / 2))] = ShaftStop::On;
			}
			if (ShaftStop::On == out.down && (Direction::Down == canbinDirection || HighterActiveFloorCount(tmpStates, canbinFloor)==0))
			{
				out.down = ShaftStop::Off;
				tmpStatseInside[CAP(1, myFloorCount, (unsigned int)((canbinFloor + 1 - 0.5) / 2))] = ShaftStop::On;
			}

		}
		
		std::vector<ShaftStop> tmpStatseOutsideUion;
		for (int i = 0; i < myFloorCount; ++i)
		{
			tmpStatseOutsideUion.push_back((ShaftStop)(tmpStatseOutside[i].up & tmpStatseOutside[i].down));
		}
		hardness_waiting += ActiveFloorCount(tmpStatseOutsideUion);
		hardness_traveling += ActiveFloorCount(tmpStatseInside);
		tmpStates = GetAllStates(tmpStatseInside, tmpStatseOutside);
		activeCount = ActiveFloorCount(tmpStates);
	}

	return hardness_waiting+ hardness_traveling;

}

unsigned int
Elevator::LowestActiveFloor(std::vector<ShaftStop>& targetStates) const
{
	for (int i = 0; i < myCurrentFloor; ++i)
	{
		if (ShaftStop::On == targetStates[i])
		{
			return i + 1;
		}
	}
	return -1;
}

unsigned int
Elevator::HighestActiveFloor(std::vector<ShaftStop>& targetStates) const
{
	for (int i = myFloorCount-1; i > 0; --i)
	{
		if (ShaftStop::On == targetStates[i])
		{
			return i + 1;
		}
	}
	return -1;
}

unsigned int							
Elevator::ActiveFloorCount(std::vector<ShaftStop>& targetStates) const
{
	int count = 0;
	for (int i = 0; i < myFloorCount; ++i)
	{
		if (ShaftStop::On == targetStates[i])
		{
			++count;
		}
	}
	return count;
}

unsigned int
Elevator::LowerActiveFloorCount(
	std::vector<ShaftStop>& targetStates, unsigned int floor) const 
{
	int count = 0;
	for (int i = 0; i < std::min(floor,myFloorCount)-1; ++i)
	{
		if (ShaftStop::On == targetStates[i])
		{
			++count;
		}
	}
	return count;
}
unsigned int
Elevator::HighterActiveFloorCount(
	std::vector<ShaftStop>& targetStates, unsigned floor) const 
{
	int count = 0;
	for (int i = floor; i < myFloorCount; ++i)
	{
		if (ShaftStop::On == targetStates[i])
		{
			++count;
		}
	}
	return count;
}

void 
Elevator::CleanStatesOutside()
{
	for (int i = 0; i < myFloorCount; i++) 
	{
		myStatesOutside[i].down = ShaftStop::Off;
		myStatesOutside[i].up = ShaftStop::Off;
	}
}

void 
Elevator::getCallCommand(unsigned int calledFloor, Direction direction)
{
	auto& outControl=myStatesOutside[calledFloor - 1];
	if (Direction::Up == direction) 
	{
		outControl.up = ShaftStop::On;
	}
	if (Direction::Down == direction)
	{
		outControl.down = ShaftStop::On;
	}
}
