#include "Quorum.h"

class CGame1 : public CGame
{
	private:
		CSpriteBatch* m_pSpriteBatch;
		CQuorumSceneManager* m_pSceneManager;
		char m_sBuffer[512];
		float masterVolume;

	protected:
		#pragma region Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(CGameTime*);
		virtual void Draw(CGameTime*);
		#pragma endregion
};