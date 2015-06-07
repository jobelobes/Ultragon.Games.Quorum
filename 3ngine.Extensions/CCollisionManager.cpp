#include "3ngine.Extensions.h"

#pragma region Static Variables
ISceneManager* CCollisionManager::g_pSceneManager = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
CCollisionManager::CCollisionManager(CGame* game)
	:CGameComponent(game)
{
	game->Get_Services()->AddService(this);

	this->m_iGridSize = 100;
}

CCollisionManager::~CCollisionManager(){}
#pragma endregion

#pragma region Methods
ContainmentType CCollisionManager::CheckCollision(ICollidableUnit* obj1, ICollidableUnit* obj2)
{
	ContainmentType containment = ContainmentType::Disjoint;
	containment = obj1->Get_BoundingBox()->Contains(obj2->Get_BoundingBox());
	if(containment != ContainmentType::Disjoint)
	{
		IAdvancedCollidableUnit* advObj1 = dynamic_cast<IAdvancedCollidableUnit*>(obj1);
		IAdvancedCollidableUnit* advObj2 = dynamic_cast<IAdvancedCollidableUnit*>(obj2);
		if(advObj1 == NULL && advObj2 == NULL)
			return containment;
		else if(advObj1 == NULL)
			containment = advObj2->Get_BoundingPolygon()->Contains(obj1->Get_BoundingBox());
		else
			containment = advObj1->Get_BoundingPolygon()->Contains(obj2->Get_BoundingBox());
	}
	return containment;
}

int CCollisionManager::ComputeHash(D3DXVECTOR3 pos)
{
	const int h1 = 0x8da6b343;
	const int h2 = 0xd8163841;
	const int h3 = 0xcd1ab31f;

	float n = h1 * pos.x + h2 * pos.y + h3 * pos.z;
	n = fmodf(n, this->m_iGridSize);
	if(n < 0) 
		n += this->m_iGridSize;
	return n;
}
#pragma endregion

#pragma region ICollisionService Methods
bool CCollisionManager::HasCollided(ICollidableUnit* queryObject)
{
	/*__int64 freq, cTime, eTime;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	::QueryPerformanceCounter((LARGE_INTEGER*)(&cTime));
	double spc = 1000.0 / freq;*/
	
	int n = this->ComputeHash(queryObject->Get_BoundingBox()->Min);	
	
	int count = this->m_pGridItemCount[n];
	for(int i = 0; i < count; i++)
	{
		if(this->CheckCollision(queryObject, this->m_pGrid[n][i]) != ContainmentType::Disjoint && this->m_pGrid[n][i]->CollisionOccured(queryObject))
			return true;
	}

	//::QueryPerformanceCounter((LARGE_INTEGER*)(&eTime));
	//printf("Has Collided[%f]\n", (eTime - cTime) * spc);
	return false;
}
#pragma endregion

#pragma region Overriden Methods
void CCollisionManager::Initialize()
{
	if(CCollisionManager::g_pSceneManager == NULL)
		CCollisionManager::g_pSceneManager = (ISceneManager*)this->Get_Game()->Get_Services()->GetService("ISceneManager");

	this->m_pGridItemCount = new unsigned int[this->m_iGridSize];
	this->m_pGrid = new std::vector<ICollidableUnit*>[this->m_iGridSize];
}

void CCollisionManager::Update(CGameTime* gameTime)
{
	/*__int64 freq, cTime, eTime;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	::QueryPerformanceCounter((LARGE_INTEGER*)(&cTime));
	double spc = 1000.0 / freq;*/

	// clear out the grid
	ZeroMemory(this->m_pGridItemCount, sizeof(unsigned int) * this->m_iGridSize);

	// put all the objects into their appropriate bins
	int n;
	ICollidableUnit* component = NULL;
	std::vector<CDrawableGameComponent*>* components = this->Get_Game()->Get_Components()->Get_DrawableGameComponents();
	for(int i = 0; i < components->size(); i++)
	{
		// don't add any non-drawable objects
		if(!(*components)[i]->Get_Visible())
			continue;

		component = dynamic_cast<ICollidableUnit*>((*components)[i]);
		if(component != NULL)
		{
			n = this->ComputeHash(component->Get_BoundingBox()->Min);
			if(this->m_pGridItemCount[n] == 0)
				this->m_pGrid[n].clear();
			this->m_pGrid[n].push_back(component);
			this->m_pGridItemCount[n]++;
		}
	}

	//::QueryPerformanceCounter((LARGE_INTEGER*)(&eTime));
	//printf("Update of Collision Manager[%f]\n", (eTime - cTime) * spc);
}
#pragma endregion