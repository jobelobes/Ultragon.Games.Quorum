#include "3ngine.Core.h"

#pragma region Static Variables
CEffect* CBoundingPolygon::g_pEffect = NULL;
CTexture2D* CBoundingPolygon::g_pTexture = NULL;
#pragma endregion

#pragma region Constructors and Finalizers
CBoundingPolygon::CBoundingPolygon()
{
	
	D3DXMatrixIdentity(&this->m_sTransform);
}

CBoundingPolygon::~CBoundingPolygon(){}
#pragma endregion

#pragma region Methods
ContainmentType CBoundingPolygon::CheckLineIntersection(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d)
{
	float f = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);
	if(abs(f) < std::numeric_limits<float>::epsilon())
		return ContainmentType::Disjoint;
	
	float g = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);

	if(f > 0 && (g < 0 || g > f))
		return ContainmentType::Disjoint;
	else if (f < 0 && (g > 0 || g < f))
		return ContainmentType::Disjoint;
		
	float h = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);

	if(f > 0 && (h < 0 || h > f))
		return ContainmentType::Disjoint;
	else if(f < 0 && (h > 0 || h < f))
		return ContainmentType::Disjoint;

	return ContainmentType::Intersects;
}

ContainmentType CBoundingPolygon::Contains(CBoundingPolygon* polygon)
{
	int count = this->m_pSegments.size();
	int pcount = polygon->m_pSegments.size();
	for(int i = 0; i < count; i++)
	{	
		D3DXVECTOR2 a = this->m_pSegments[i];
		D3DXVECTOR2 b = this->m_pSegments[(i + 1) % count];

		for(int j = 0; j < pcount; j++)
		{
			D3DXVECTOR2 c = polygon->m_pSegments[j];
			D3DXVECTOR2 d = polygon->m_pSegments[(j + 1) % pcount];
			if(this->CheckLineIntersection(a, b, c, d) == ContainmentType::Intersects)
				return ContainmentType::Intersects;
		}
	}

	return ContainmentType::Disjoint;
}

ContainmentType CBoundingPolygon::Contains(CBoundingBox* box)
{
	D3DXVECTOR2* corners = new D3DXVECTOR2[4];
	box->GetCorners(&corners);

	int count = this->m_pSegments.size();
	for(int i = 0; i < count; i++)
	{	
		D3DXVECTOR2 a = this->m_pSegments[i];
		D3DXVECTOR2 b = this->m_pSegments[(i + 1) % count];

		for(int j = 0; j < 4; j++)
		{
			D3DXVECTOR2 c = corners[j];
			D3DXVECTOR2 d = corners[(j + 1) % 4];
			if(this->CheckLineIntersection(a, b, c, d) == ContainmentType::Intersects)
				return ContainmentType::Intersects;
		}
	}
}

ContainmentType CBoundingPolygon::Contains(D3DXVECTOR3 point)
{
	return ContainmentType::Disjoint;
}

bool CBoundingPolygon::Intersects(CBoundingPolygon box)
{
   return false;
}
#pragma endregion

#pragma region Operator Methods
bool CBoundingPolygon::operator==(const CBoundingPolygon& value)
{
	return false;
}

bool CBoundingPolygon::operator!=(const CBoundingPolygon& value)
{
	return true;
}
#pragma endregion

#pragma region Static Methods
CMesh* CBoundingPolygon::BuildMesh(CGraphicsDevice* device, std::vector<D3DXVECTOR2>* segments)
{
	int count = (*segments).size();

	// create vertices
	UINT* indices = new UINT[count + 1];
	VertexPositionColor* vertices = new VertexPositionColor[count];
	for(int i = 0; i < count; i++)
	{
		vertices[i].Position.x = (*segments)[i].x;
		vertices[i].Position.y = 0;
		vertices[i].Position.z = (*segments)[i].y;
		vertices[i].Color = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
		indices[i] = i;
	}
	indices[count] = 0;

	CMesh* mesh = new CMesh(device);
	mesh->SetVertices((void*)vertices, count);

	mesh->SetIndices((UINT*)indices, count + 1);

	mesh->Create((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColor::VertexElements, VertexPositionColor::VertexElementCount);

	delete [] vertices;
	delete [] indices;

	return mesh;
}

void CBoundingPolygon::Draw(CGraphicsDevice* device, ICameraService* camera)
{
	if(CBoundingPolygon::g_pMesh == NULL)
		CBoundingPolygon::g_pMesh = CBoundingPolygon::BuildMesh(device, &this->m_pSegments);

	if(CBoundingPolygon::g_pEffect == NULL)
		CBoundingPolygon::g_pEffect = CEffect::Load(device, "..\\Assets\\Effects\\BoundingEffect.fx");

	device->Set_VertexDeclaration((D3D10_INPUT_ELEMENT_DESC*)VertexPositionColorTexture.VertexElements, VertexPositionColorTexture::VertexElementCount);

	CBoundingPolygon::g_pEffect->Set_View(&camera->Get_View());
	CBoundingPolygon::g_pEffect->Set_World(&this->m_sTransform);
	CBoundingPolygon::g_pEffect->Set_Projection(&camera->Get_Projection());
	CBoundingPolygon::g_pEffect->Begin();
	{
		ID3D10EffectTechnique* technique = CBoundingPolygon::g_pEffect->Get_CurrentTechnique();

		D3D10_TECHNIQUE_DESC td;
		technique->GetDesc(&td);
		for( UINT p = 0; p < td.Passes; ++p )
		{
			technique->GetPassByIndex(p)->Apply(0);
			CBoundingPolygon::g_pMesh->Draw();
		}
	}
	CBoundingPolygon::g_pEffect->End();
}
#pragma endregion