#pragma once

#include <mutex>
#include <vector>
#include "Elevator.h"
#include "Messages.h"



class Elevators
{
public:
							Elevators();

	void					Start(const Configuration& config);

	void					OnMessageElevatorCall(
								const MessageElevatorCall&				aMessage);
	void					OnMessageElevatorRequest(
								const MessageElevatorRequest&			aMessage);

	void					OnMessageElevatorStep(
								const MessageElevatorStep&				aMessage);

private:


	std::vector<Elevator>	myElevators;
	std::vector<Shaft>		myShafts;
	unsigned int			myFloorCount;

	void					ElevatorsCleanCalls();
	void					DistributeCalls();
	bool					ShaftsHasWork(const std::vector<Shaft> & shafts);
};
