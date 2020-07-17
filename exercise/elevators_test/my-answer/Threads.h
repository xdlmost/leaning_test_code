#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class Threads
{
public:
		static Threads&						GetInstance()
		{
			static Threads instance;
			return instance;
		}

		void								Start();
		void								AddElevatorWork(
												std::function<void()>	aWork);
		void								AddHumanWork(
												std::function<void()>	aWork);

		void								Wait();
		void								Exit();

private:

											Threads():exitFlag(false){};

	void									ElevatorWorker(
												unsigned int delay );


	std::thread								myElevatorsThread;
	std::vector<std::function<void()>>		myElevatorsWork;
	std::condition_variable					myElevatorsCv;
	std::mutex								myElevatorsMutex;

	void									HumansWorker(
												unsigned int delay );

	void Delay								(const unsigned int delay );

	std::thread								myHumansThread;
	std::vector<std::function<void()>>		myHumansWork;
	std::condition_variable					myHumansCv;
	std::mutex								myHumansMutex;

	std::mutex								myExitsMutex;
	bool									exitFlag;

};
