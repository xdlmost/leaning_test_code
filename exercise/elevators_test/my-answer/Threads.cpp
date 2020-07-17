#include <chrono>

#include "Threads.h"

// Shrink this number to make the system go faster.
static const unsigned int locThreadWaitTimeMs = 1000;

void
Threads::ElevatorWorker(unsigned int delay)
{
	//after a specific delay
	if (delay > 0)
	{
		Delay(delay);
	}

	std::vector<std::function<void()>> work;

	while(!exitFlag)
	{
		{
			std::unique_lock<std::mutex> lock(myElevatorsMutex);
			myElevatorsCv.wait_for(lock, std::chrono::milliseconds(locThreadWaitTimeMs));

			work.swap(myElevatorsWork);
			myElevatorsWork.clear();
		}

		for(auto wrk : work)
		{
			wrk();
		}
	}
}

void
Threads::HumansWorker(unsigned int delay)
{
	//after a specific delay
	if (delay > 0)
	{
		Delay(delay);
	}

	std::vector<std::function<void()>> work;

	while(!exitFlag)
	{
		{
			std::unique_lock<std::mutex> lock(myHumansMutex);
			myHumansCv.wait_for(lock, std::chrono::milliseconds(locThreadWaitTimeMs));

			work.swap(myHumansWork);
			myHumansWork.clear();
		}

		for(auto wrk : work)
		{
			wrk();
		}
	}
}

void Threads::Delay(const unsigned int delay)
{

	std::mutex delayMutex;
	std::unique_lock<std::mutex> delaylock(delayMutex);
	std::condition_variable delayCv;
	delayCv.wait_for(delaylock, std::chrono::milliseconds(delay));
}

void 
Threads::Exit()
{
	std::unique_lock<std::mutex> lock(myExitsMutex);
	exitFlag = true;
	lock.unlock();
}

void
Threads::Start()
{
	myElevatorsThread = std::thread(&Threads::ElevatorWorker, this,1000);
	myHumansThread = std::thread(&Threads::HumansWorker, this,0);
}

void
Threads::Wait()
{
	myElevatorsThread.join();
	myHumansThread.join();
}

void
Threads::AddElevatorWork(
	std::function<void()>	aWork)
{
	{
		std::unique_lock<std::mutex> lock(myElevatorsMutex);
		myElevatorsWork.emplace_back(aWork);
	}

	myElevatorsCv.notify_all();
}

void
Threads::AddHumanWork(
	std::function<void()>	aWork)
{
	{
		std::unique_lock<std::mutex> lock(myHumansMutex);
		myHumansWork.emplace_back(aWork);
	}

	myHumansCv.notify_all();
}