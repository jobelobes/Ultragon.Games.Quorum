#include "3ngine.Core.h"

#pragma region Constructors and Finalizers
CBoundingBox::CBoundingBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	this->Min = D3DXVECTOR3(min.x, min.y, min.z);
	this->Max = D3DXVECTOR3(max.x, max.y, max.z);
}

CBoundingBox::~CBoundingBox(){}
#pragma endregion

#pragma region Methods
ContainmentType CBoundingBox::Contains(CBoundingBox* box)
{
	if ((this->Max.x < box->Min.x) || (this->Min.x > box->Max.x))
		return ContainmentType::Disjoint;
    if ((this->Max.y < box->Min.y) || (this->Min.y > box->Max.y))
		return ContainmentType::Disjoint;
    if ((this->Max.z < box->Min.z) || (this->Min.z > box->Max.z))
		return ContainmentType::Disjoint;
    if ((((this->Min.x <= box->Min.x) && (box->Max.x <= this->Max.x)) 
		&& ((this->Min.y <= box->Min.y) && (box->Max.y <= this->Max.y))) 
		&& ((this->Min.z <= box->Min.z) && (box->Max.z <= this->Max.z)))
		return ContainmentType::Contains;
	return ContainmentType::Intersects;
}

ContainmentType CBoundingBox::Contains(D3DXVECTOR3 point)
{
    if ((((this->Min.x <= point.x) && (point.x <= this->Max.x)) && 
		((this->Min.y <= point.y) && (point.y <= this->Max.y))) && 
		((this->Min.z <= point.z) && (point.z <= this->Max.z)))
		return ContainmentType::Contains;
	return ContainmentType::Disjoint;
}

void CBoundingBox::GetCorners(D3DXVECTOR2** corners)
{
	(*corners)[0].x = this->Min.x;
	(*corners)[0].y = this->Min.z;

	(*corners)[1].x = this->Max.x;
	(*corners)[1].y = this->Min.z;

	(*corners)[2].x = this->Max.x;
	(*corners)[2].y = this->Max.z;

	(*corners)[3].x = this->Min.x;
	(*corners)[3].y = this->Max.z;
}

bool CBoundingBox::Intersects(CBoundingBox* box)
{
    if ((this->Max.x < box->Min.x) || (this->Min.x > box->Max.x))
        return false;
    if ((this->Max.y < box->Min.y) || (this->Min.y > box->Max.y))
        return false;
    return ((this->Max.z >= box->Min.z) && (this->Min.z <= box->Max.z));
}

void CBoundingBox::Offset(D3DXVECTOR3 offset)
{
	this->Min += offset;
	this->Max += offset;
}

#pragma endregion

#pragma region Operator Methods
bool CBoundingBox::operator==(const CBoundingBox& value)
{
	return (this->Min.x == value.Min.x)
		&& (this->Min.y == value.Min.y)
		&& (this->Min.z == value.Min.z)
		&& (this->Max.x == value.Max.x)
		&& (this->Max.y == value.Max.y)
		&& (this->Max.z == value.Max.z);
}

bool CBoundingBox::operator!=(const CBoundingBox& value)
{
	return (this->Min.x != value.Min.x)
		|| (this->Min.y != value.Min.y)
		|| (this->Min.z != value.Min.z)
		|| (this->Max.x != value.Max.x)
		|| (this->Max.y != value.Max.y)
		|| (this->Max.z != value.Max.z);
}
#pragma endregion

#pragma region Static Methods
CBoundingBox* CBoundingBox::CreateMerged(CBoundingBox original, CBoundingBox additional)
{
	D3DXVECTOR3 min;
	min.x = min(original.Min.x,additional.Min.x);
	min.y = min(original.Min.y,additional.Min.y);
	min.z = min(original.Min.z,additional.Min.z);

	D3DXVECTOR3 max;
	max.x = max(original.Max.x,additional.Max.x);
	max.y = max(original.Max.y,additional.Max.y);
	max.z = max(original.Max.z,additional.Max.z);

    return new CBoundingBox(min, max);
}
#pragma endregion