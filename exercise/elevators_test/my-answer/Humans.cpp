#include "MessageBus.h"

#include "Humans.h"

Human::Human(
	const unsigned int		aFloor,
	const unsigned int		aDestinationFloor)
	: myFloor(aFloor)
	, myDestinationFloor(aDestinationFloor)
	, myState(HumanState_Idle)
	, myWaitingCounter(0)
	, myTravelingCounter(0)
	, myElevatorId(0)
{
	if (myFloor == myDestinationFloor)
	{
		Log("[Human][Error] Same floor and destination floor: ", myFloor);
		assert(false);
	}
}

HumanState
Human::GetState() const
{
	return myState;
}

void
Human::SetStateWaiting()
{
	myState = HumanState_Waiting;
}

void
Human::SetStateTraveling()
{
	myState = HumanState_Traveling;
}

void
Human::SetStateArrived()
{
	myState = HumanState_Arrived;
}

void
Human::Step()
{
	if (myState == HumanState_Traveling)
	{
		++myTravelingCounter;
	}
	if (myState == HumanState_Waiting)
	{
		++myWaitingCounter;
	}
}

unsigned int
Human::GetWaitingCounter() const
{
	return myWaitingCounter;
}

unsigned int
Human::GetTravelingCounter() const
{
	return myTravelingCounter;
}

unsigned int Human::GetElevatorId()
{
	return myElevatorId;
}

void 
Human::SetElevatorId(unsigned int elevatorId)
{
	myElevatorId = elevatorId;
}

///

Humans::Humans():readyFlag(false)
{
}

void
Humans::Start(const Configuration& config, std::function<void()> exitFunc)
{
	REGISTER_HUMAN(MessageElevatorReady,		Humans::OnMessageElevatorReady);
	REGISTER_HUMAN(MessageElevatorArrived,		Humans::OnMessageElevatorArrived);
	REGISTER_HUMAN(MessageHumanStep,			Humans::OnMessageHumanStep);
	REGISTER_HUMAN(MessageHumanDone,			Humans::OnMessageHumanDone); //clean exit
	myExitFunc = exitFunc;
	config.buildFloor;
	for (size_t i = 0; i < config.humansCount; ++i)
	{
		unsigned int from = Random(1, config.buildFloor);
		unsigned int to = Random(1, config.buildFloor);
		while (from == to)
		{
			to = Random(1, config.buildFloor);
		}
		myHumans.push_back(Human(from, to));
	}

}

void
Humans::OnMessageElevatorReady(
	const MessageElevatorReady&		aMessage)
{
	Log("[Humans] Elevator system ready");

	readyFlag = true;

}

void
Humans::OnMessageElevatorArrived(
	const MessageElevatorArrived&	aMessage)
{
	Log("[Humans] Elevator arrived at floor:", aMessage.myFloor);
	for (Human& human : myHumans)
	{
		if (HumanState_Traveling == human.GetState() && aMessage.myElevatorId == human.GetElevatorId() && human.myDestinationFloor == aMessage.myFloor)
		{
			human.SetStateArrived();
		}
		else if (HumanState_Waiting == human.GetState() && human.myFloor == aMessage.myFloor &&DirectionEx::exNoWay != aMessage.myDirectionEx)
		{
			if ((DirectionEx::exUp == aMessage.myDirectionEx && human.myDestinationFloor > human.myFloor) ||
				(DirectionEx::exDown == aMessage.myDirectionEx && human.myDestinationFloor < human.myFloor) ||
				DirectionEx::exTwoWay == aMessage.myDirectionEx) 
			{
				human.SetStateTraveling();
				human.SetElevatorId(aMessage.myElevatorId);
				MessageElevatorRequest message = { aMessage.myElevatorId,human.myDestinationFloor };
				SEND_TO_ELEVATORS(message)
			}

		}
	}
	Log("@@Humans::OnMessageElevatorArrived");
}

void
Humans::OnMessageHumanStep(
	const MessageHumanStep&		aMessage)
{	
	Log("[Humans] Step");

	for (Human& human : myHumans)
	{
		human.Step();
	}
	TryExit();

	PrivPrintTimers();

	// Implement me!
	if(readyFlag)
	{
		for (Human& human : myHumans)
		{
			if (HumanState_Idle == human.GetState())
			{
				if (human.myFloor == human.myDestinationFloor)
				{
					human.SetStateArrived();
				}
				else
				{
					MessageElevatorCall elevatorCall;
					elevatorCall.myFloor = human.myFloor;
					elevatorCall.myDirection = human.myDestinationFloor > human.myFloor ? Direction::Up : Direction::Down;
					human.SetStateWaiting();
					SEND_TO_ELEVATORS(elevatorCall);
				}
			}
		}
	}

	Log("@@Humans::OnMessageHumanStep");

	//MessageHumanStep message;
	//SEND_TO_HUMANS(message);
}
void
Humans::OnMessageHumanDone(
	const MessageHumanDone&			aMessage)
{
	myExitFunc();
}
void
Humans::PrivPrintTimers()
{
	unsigned int sumWaiting = 0;
	unsigned int sumTraveling = 0;
	unsigned int humansArrivedCounter = 0;

	for (const Human& human : myHumans)
	{
		if (human.GetState() != HumanState_Arrived)
		{
			continue;
		}

		sumWaiting += human.GetWaitingCounter();
		sumTraveling += human.GetTravelingCounter();
		++humansArrivedCounter;
	}

	if (humansArrivedCounter == 0)
	{
		return;
	} 


	Log("[Stats] Average wait time (steps): ", (sumWaiting / static_cast<float>(humansArrivedCounter)));
	Log("[Stats] Average travel time (steps): ", (sumTraveling / static_cast<float>(humansArrivedCounter)));
}

void Humans::TryExit()
{
	unsigned int humansArrivedCounter = 0;
	for (const Human& human : myHumans)
	{
		if (human.GetState() == HumanState_Arrived)
		{
			++humansArrivedCounter;
		}
	}
	if (myHumans.size() == humansArrivedCounter)
	{
		MessageHumanDone message;
		SEND_TO_HUMANS(message);
	}
}
