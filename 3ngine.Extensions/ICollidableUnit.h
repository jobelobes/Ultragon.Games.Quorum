#include "3ngine.Extensions.h"

class EXTENSIONS_API ICollidableUnit
{
	public:
		#pragma region Properties
		virtual CBoundingBox* Get_BoundingBox() = 0;

		virtual bool CollisionOccured(ICollidableUnit*) = 0;
		#pragma endregion
};

class EXTENSIONS_API IAdvancedCollidableUnit
{
	public:
		#pragma region Properties
		virtual CBoundingPolygon* Get_BoundingPolygon() = 0;
		#pragma endregion
};