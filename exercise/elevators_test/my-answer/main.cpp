#include <condition_variable>
#include <mutex>
#include "Elevators.h"
#include "Humans.h"
#include "MessageBus.h"
#include "Threads.h"

Elevators elevators;
Humans humans;
#define FLOOR_COUNT 10
#define HUMANS_COUNT 10
#define ELEVATORS_COUNT 2
const Configuration config{ FLOOR_COUNT,HUMANS_COUNT ,ELEVATORS_COUNT };


void
RunElevators()
{
	elevators.Start(config);
}

void
RunHumans()
{
	humans.Start(config, [=]() { Threads::GetInstance().Exit(); } );
}


int main()
{

	Threads::GetInstance().Start();
	MessageBus::GetInstance();

	Threads::GetInstance().AddHumanWork(std::bind(&RunHumans));
	Threads::GetInstance().AddElevatorWork(std::bind(&RunElevators));

	Threads::GetInstance().Wait();

	return 0;
}