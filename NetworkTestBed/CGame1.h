#include "3ngine.Core.h"

class CGame1 : public CGame
{
	protected:
		#pragma region Methods
		virtual void Initialize();
		virtual void LoadContent();
		virtual void Update(CGameTime*);
		virtual void Draw(CGameTime*);
		#pragma endregion
};