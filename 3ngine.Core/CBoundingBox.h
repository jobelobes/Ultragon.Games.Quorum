#include "3ngine.Core.h"

enum ContainmentType
{
    Disjoint,
    Contains,
    Intersects
};

class CORE_API CBoundingBox
{
	public:
		#pragma region Variables
		D3DXVECTOR3 Min;
		D3DXVECTOR3 Max;
		#pragma endregion

		#pragma region Constructors and Finalizers
		CBoundingBox(D3DXVECTOR3, D3DXVECTOR3);
		~CBoundingBox();
		#pragma endregion

		#pragma region Methods
		ContainmentType Contains(CBoundingBox*);
		ContainmentType Contains(D3DXVECTOR3);
		void GetCorners(D3DXVECTOR2**);
		bool Intersects(CBoundingBox*);
		void Offset(D3DXVECTOR3);
		#pragma endregion

		#pragma region Static Methods
		static CBoundingBox* CreateMerged(CBoundingBox, CBoundingBox);
		#pragma endregion

		#pragma region Opertator Overriden Methods
		bool operator==(const CBoundingBox& value);
		bool operator!=(const CBoundingBox& value);
		#pragma endregion
};
