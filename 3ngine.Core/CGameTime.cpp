#include "3ngine.Core.h"

#pragma region Properties
float CGameTime::Get_ElapsedTime()
{
	return this->m_dDeltaTime;
}

float CGameTime::Get_TotalTime()
{
	return (this->m_iCurrTime - this->m_iStartTime) * this->m_dSecondsPerCount;
}
#pragma endregion

#pragma region Constructors and Finalizers
CGameTime::CGameTime()
{
	this->m_dDeltaTime = 0;
	this->m_iPausedTime = 0;
	this->m_iCurrTime = 0;
	this->m_iStartTime = 0;
	this->m_bStopped = false;

	this->Reset();
}
#pragma endregion

#pragma region Methods
void CGameTime::Tick()
{
	if(this->m_bStopped)
	{
		this->m_dDeltaTime = 0.0;
		return;
	}

	::QueryPerformanceCounter((LARGE_INTEGER*)(&this->m_iCurrTime));

	this->m_dDeltaTime = (this->m_iCurrTime - this->m_iPrevTime) * this->m_dSecondsPerCount;
	this->m_iPrevTime = this->m_iCurrTime;

	if(this->m_dDeltaTime < 0.0)
		this->m_dDeltaTime = 0.0;
}

void CGameTime::Reset()
{
	__int64 countsPerSec;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	this->m_dSecondsPerCount = 1.0 / countsPerSec;
	this->m_iBaseTime = this->m_iCurrTime;
	this->m_iPrevTime = this->m_iCurrTime;
	this->m_iCurrTime = this->m_iCurrTime;
	this->m_iStartTime = this->m_iCurrTime;
	this->m_iStopTime = 0;
	this->m_bStopped = false;
}
#pragma endregion