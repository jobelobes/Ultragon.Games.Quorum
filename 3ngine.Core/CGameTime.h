#include "3ngine.Core.h"

class CORE_API CGameTime
{
	private:
		double m_dDeltaTime;
		double m_dSecondsPerCount;
		__int64 m_iBaseTime;
		__int64 m_iPausedTime;
		__int64 m_iStartTime;
		__int64 m_iStopTime;
		__int64 m_iPrevTime;
		__int64 m_iCurrTime;

		bool m_bStopped;
	
	public:
		#pragma region Properties
		float Get_ElapsedTime();
		float Get_TotalTime();
		#pragma endregion

		#pragma region Constructors and Finalizers
		CGameTime();
		#pragma endregion

		#pragma region Methods
		void Tick();
		void Reset();
		#pragma endregion
};