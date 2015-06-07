#include "Quorum.h"

class CGame1 : public CGame
{
	private:
		CSpriteBatch* m_pSpriteBatch;
		char m_sBuffer[256];

		CWindowsInput* m_pInputService;

	protected:
		#pragma region Methods
		virtual void Initialize();
		virtual void Update(CGameTime*);
		virtual void Draw(CGameTime*);
		#pragma endregion
};