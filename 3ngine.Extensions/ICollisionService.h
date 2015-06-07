#include "3ngine.Extensions.h"

class EXTENSIONS_API ICollisionService : public IGameService
{
	public:
		#pragma region IGameService Properties
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods
		virtual bool HasCollided(ICollidableUnit*) = 0;
		#pragma endregion
};