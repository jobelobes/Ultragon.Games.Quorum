#include "3ngine.Core.h"

class CORE_API IXACTEngineService : public IGameService
{
	public:
		virtual void PlaySound(XACTINDEX&) = 0;
		virtual void StopSound(XACTINDEX, DWORD) = 0;
		virtual void GetCueIndex(LPCSTR, XACTINDEX*) = 0;

		virtual void PauseEngine(XACTCATEGORY, bool) = 0;
		virtual void StopEngine(XACTCATEGORY, DWORD) = 0;		
		virtual void Set_Volume(XACTCATEGORY, XACTVOLUME) = 0;
		virtual XACTCATEGORY Get_Category(PCSTR) = 0;

		virtual void PlayWave(XACTINDEX, DWORD, DWORD, XACTLOOPCOUNT, IXACT3Wave**) = 0;
		virtual void StopWave(XACTINDEX, DWORD) = 0;
		virtual void GetWaveIndex(PCSTR, XACTINDEX*) = 0;

		virtual void LoadWaveBank(LPCSTR) = 0;
		virtual void LoadSoundBank(LPCSTR) = 0;

		#pragma region IGameService Methods
		virtual char* Get_Type();
		#pragma endregion
};
