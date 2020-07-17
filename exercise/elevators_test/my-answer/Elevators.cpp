#include "MessageBus.h"

#include "Elevators.h"
#include <float.h>

Elevators::Elevators()
{
}

void
Elevators::Start(const Configuration& config)
{
	REGISTER_ELEVATOR(MessageElevatorCall,			Elevators::OnMessageElevatorCall);
	REGISTER_ELEVATOR(MessageElevatorStep,			Elevators::OnMessageElevatorStep);
	REGISTER_ELEVATOR(MessageElevatorRequest,		Elevators::OnMessageElevatorRequest);

	myFloorCount = config.buildFloor;
	for (size_t i = 0; i < config.elevatorsCount; ++i) 
	{
		myElevators.push_back(Elevator{ 
			i+1, 
			myFloorCount,
			(unsigned int)Random(1,myFloorCount), 
			(Direction)Random(0,1)
			});
	}

	for (int i = 0; i < myFloorCount; i++)
	{
		myShafts.push_back(Shaft{ ShaftStop::Off,ShaftStop::Off });
	}

	{
		MessageElevatorReady message;
		SEND_TO_HUMANS(message);
	}


	{
		MessageElevatorStep message;
		SEND_TO_ELEVATORS(message);
	}

}

void
Elevators::OnMessageElevatorCall(
	const MessageElevatorCall&				aMessage)
{
	// Implement me!
	float hardness = FLT_MAX;

	if (Direction::Up == aMessage.myDirection) 
	{ 
		myShafts[aMessage.myFloor - 1].up=ShaftStop::On;
	}
	else if (Direction::Down == aMessage.myDirection)
	{
		myShafts[aMessage.myFloor - 1].down = ShaftStop::On;
	}
}

void
Elevators::OnMessageElevatorRequest(
	const MessageElevatorRequest&			aMessage)
{
	// Implement me!
	auto& targetElevator = myElevators[aMessage.myElevatorId-1];
	targetElevator.SelectFloor(aMessage.myFloor);
}

void
Elevators::OnMessageElevatorStep(
	const MessageElevatorStep&				aMessage)
{
	Log("[Elevators] Step");

	ElevatorsCleanCalls();
	DistributeCalls();

	for (auto& elevator : myElevators)
	{
		elevator.Step();
		auto directionEx=elevator.TryArrive();
		if (DirectionEx::exStop != directionEx)
		{
			auto arrivedFloor = elevator.CurrentFloor();
			if (DirectionEx::exUp == directionEx)
			{
				myShafts[arrivedFloor - 1].up = ShaftStop::Off;
			}
			else if (DirectionEx::exDown == directionEx)
			{
				myShafts[arrivedFloor - 1].down = ShaftStop::Off;
			}
			else if (DirectionEx::exTwoWay == directionEx)
			{
				myShafts[arrivedFloor - 1].up = ShaftStop::Off;
				myShafts[arrivedFloor - 1].down = ShaftStop::Off;
			}
			MessageElevatorArrived messageElevatorArrived;
			messageElevatorArrived.myElevatorId = elevator.Id();
			messageElevatorArrived.myDirectionEx = directionEx;
			messageElevatorArrived.myFloor = elevator.CurrentFloor();
			SEND_TO_HUMANS(messageElevatorArrived)
		}
	}

	MessageElevatorStep message;
	SEND_TO_ELEVATORS(message);

	MessageHumanStep messageHumanStep;
	SEND_TO_HUMANS(messageHumanStep)
}	

void 
Elevators::ElevatorsCleanCalls()
{
	for (auto & elevator : myElevators)
	{
		elevator.CleanStatesOutside();
	}
}

void 
Elevators::DistributeCalls()
{
	std::vector<Shaft> tmpShafts(myShafts);
	while (ShaftsHasWork(tmpShafts))
	{
		float hardness = FLT_MAX;
		Direction direction;
		unsigned int floor;
		Elevator * pElevator = NULL;
		for (auto & elevator : myElevators)
		{
			for (int i = 0; i < myFloorCount; ++i)
			{
				auto testingFloor = i + 1;
				auto &shaft = tmpShafts[i];

				if (ShaftStop::On == shaft.down)
				{
					float newHardness = elevator.getHardness(testingFloor, Direction::Down);
					if (newHardness < hardness)
					{
						hardness = newHardness;
						pElevator = &elevator;
						direction = Direction::Down;
						floor = testingFloor;
					}
				}
				if (ShaftStop::On == shaft.up)
				{
					float newHardness = elevator.getHardness(testingFloor, Direction::Up);
					if (newHardness < hardness)
					{
						hardness = newHardness;
						pElevator = &elevator;
						direction = Direction::Up;
						floor = testingFloor;
					}
				}
			}
		}
		pElevator->getCallCommand(floor, direction);

		if (Direction::Up == direction)
		{
			tmpShafts[floor - 1].up = ShaftStop::Off;
		}
		if (Direction::Down == direction)
		{
			tmpShafts[floor - 1].down = ShaftStop::Off;
		}
	}
}

bool Elevators::ShaftsHasWork(const std::vector<Shaft>& shafts)
{
	for (auto & shaft : shafts)
	{
		if (ShaftStop::On == shaft.up || ShaftStop::On == shaft.down)
		{
			return true;
		}
	}
	return false;
}
