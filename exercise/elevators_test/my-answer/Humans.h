#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>
#include "Messages.h"

enum HumanState
{
	HumanState_Idle = 0,
	HumanState_Waiting,
	HumanState_Traveling,
	HumanState_Arrived
};

class Human
{
public:
							Human(
								const unsigned int	aFloor,
								const unsigned int	aDestinationFloor);

	unsigned int			myFloor;
	unsigned int			myDestinationFloor;
	unsigned int			myElevatorId;

	HumanState				GetState() const;
	void					SetStateWaiting();
	void					SetStateTraveling();
	void					SetStateArrived();

	void					Step();
	unsigned int			GetWaitingCounter() const;
	unsigned int			GetTravelingCounter() const;
	unsigned int			GetElevatorId();
	void					SetElevatorId(
								unsigned int		elevatorId);

private:
	HumanState				myState;
	unsigned int			myWaitingCounter;
	unsigned int			myTravelingCounter;
};

class Humans
{
public:
							Humans();

	void					Start(const Configuration& config, std::function<void()> exitFunc);

	void					OnMessageElevatorReady(
								const MessageElevatorReady&		aMessage);

	void					OnMessageElevatorArrived(
								const MessageElevatorArrived&	aMessage);

	void					OnMessageHumanStep(
								const MessageHumanStep&			aMessage);
	void					OnMessageHumanDone(
								const MessageHumanDone&			aMessage);

private:
	std::vector<Human>		myHumans;
	std::function<void()>   myExitFunc;
	void					PrivPrintTimers();
	void					TryExit();
	bool					readyFlag;
};
