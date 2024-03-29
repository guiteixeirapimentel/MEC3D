#include "Timer.h"
#include <iostream>

Timer::Timer()
{
	QueryPerformanceFrequency(&cCountsPerSec);
}

Timer::~Timer()
{}

// returns the id of the timer
int Timer::SetTimer(const std::string& label)
{
	cTimers.push_back({});
	cLabels.push_back(label);

	int cLastIdGiven = cTimers.size() - 1;

	return cLastIdGiven;
}

void Timer::Tick(int id)
{
	if (id < cLastIdGiven || id >= 0)
	{
		QueryPerformanceCounter(&cTimers[id].tick);
	}
#ifdef _DEBUG
	else
	{

		std::cout << "Erro id " << id << " nao existe (timer.h)" << std::endl;

	}
#endif
}
void Timer::Tock(int id)
{
	if (id < cLastIdGiven || id >= 0)
	{
		QueryPerformanceCounter(&cTimers[id].tock);
	}
#ifdef _DEBUG
	else
	{

		std::cout << "Erro id " << id << " nao existe (timer.h)" << std::endl;

	}
#endif
}

// if id does not exist -> returns nan
float Timer::GetLastTickTock(int id) const
{
	if (id < cLastIdGiven || id >= 0)
	{
		return float(cTimers[id].tock.QuadPart - cTimers[id].tick.QuadPart) / float(cCountsPerSec.QuadPart);
	}
#ifdef _DEBUG
	else
	{

		std::cout << "Erro id " << id << " nao existe (timer.h)" << std::endl;

	}
#endif
}
// if id does not exist -> returns  "NA"
std::string Timer::GetLabel(int id) const
{
	if (id < cLastIdGiven || id >= 0)
	{
		return cLabels[id];
	}
#ifdef _DEBUG
	else
	{
		std::cout << "Erro id " << id << " nao existe (timer.h)" << std::endl;

	}
#endif
}

void Timer::WriteToCoutTickTock(int id) const
{
	if (id < cLastIdGiven || id >= 0)
	{
		const float timeSec = float(cTimers[id].tock.QuadPart - cTimers[id].tick.QuadPart) / float(cCountsPerSec.QuadPart / 1000);

		std::cout << "# Timer " << cLabels[id] << " total time in milisecs " << int(timeSec) << std::endl;


	}
#ifdef _DEBUG
	else
	{
		std::cout << "Erro id " << id << " nao existe (timer.h)" << std::endl;

	}
#endif

}

void Timer::WriteToCoutTickTockMicrosec(int id) const
{
	if (id < cLastIdGiven || id >= 0)
	{
		const double timeSec = double(cTimers[id].tock.QuadPart - cTimers[id].tick.QuadPart) / double(cCountsPerSec.QuadPart);

		std::cout << "# Timer " << cLabels[id] << " total time in microsecs " << int(timeSec * 1e6) << std::endl;


	}
#ifdef _DEBUG
	else
	{
		std::cout << "Erro id " << id << " nao existe (timer.h)" << std::endl;

	}
#endif
}

void Timer::WriteToCoutAllTickTock() const
{
	for (size_t i = 0; i < cLabels.size(); i++)
	{
		WriteToCoutTickTock(i);
	}
}