#include "3ngine.Core.h"

CEffect::CEffect(CGraphicsDevice* device)
{
	this->m_pGraphicsDevice = device;
	this->m_pCurrTechnique = NULL;
	this->m_pInputLayout = NULL;
	this->m_pEffect = NULL;
	this->m_pView = NULL;
	this->m_pWorld = NULL;
	this->m_pProjection = NULL;
}

CEffect::~CEffect()
{
	if(this->m_pEffect != NULL)
		delete this->m_pEffect;

	if(this->m_pInputLayout != NULL)
		this->m_pInputLayout->Release();
}


void CEffect::Set_View(D3DXMATRIX* value)
{
	if(!this->m_pView)
		this->m_pView = this->m_pEffect->GetVariableByName("View")->AsMatrix();

	this->m_pView->SetMatrix(*value->m);
}


void CEffect::Set_World(D3DXMATRIX* value)
{
	if(!this->m_pWorld)
		this->m_pWorld = this->m_pEffect->GetVariableByName("World")->AsMatrix();

	this->m_pWorld->SetMatrix(*value->m);
}


void CEffect::Set_Projection(D3DXMATRIX* value)
{
	if(!this->m_pProjection)
		this->m_pProjection = this->m_pEffect->GetVariableByName("Projection")->AsMatrix();

	this->m_pProjection->SetMatrix(*value->m);
}

void CEffect::SetVariable(char* key, CTexture2D* texture)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsShaderResource()->SetResource(texture->m_pShaderResView);
}

void CEffect::SetVariable(char* key, D3DXCOLOR color)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsVector()->SetFloatVectorArray((FLOAT*)color, 0, 4);
}

void CEffect::SetVariable(char* key, float value)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsScalar()->SetFloat(value);
}

void CEffect::SetVariable(char* key, int value)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsScalar()->SetInt(value);
}

void CEffect::SetVariable(char* key, D3DXVECTOR2 value)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsVector()->SetFloatVectorArray((FLOAT*)value,0, 2);
}

void CEffect::SetVariable(char* key, D3DXVECTOR3 value)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsVector()->SetFloatVectorArray((FLOAT*)value,0, 3);
}

void CEffect::SetVariable(char* key, D3DXVECTOR4 value)
{
	ID3D10EffectVariable* variable = this->m_pEffect->GetVariableByName(key);
	variable->AsVector()->SetFloatVectorArray((FLOAT*)value,0, 4);
}

ID3D10EffectTechnique* CEffect::Get_CurrentTechnique()
{
	return this->m_pCurrTechnique;
}

void CEffect::Set_Technique(char* path)
{
	this->m_pCurrTechnique = this->m_pEffect->GetTechniqueByName(path);
}

CEffect* CEffect::Load(CGraphicsDevice* device, char* path)
{
	CEffect* effect = new CEffect(device);

	HRESULT hr = D3DX10CreateEffectFromFile(
		path,
		NULL,
		NULL,
		"fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		device->m_pGraphicsDevice,
		NULL,
		NULL,
		&effect->m_pEffect,
		NULL,
		NULL);

	if(FAILED(hr))
		exit(hr);

	effect->m_pCurrTechnique = effect->m_pEffect->GetTechniqueByIndex(0);
	effect->m_pView = effect->m_pEffect->GetVariableByName("View")->AsMatrix();
	effect->m_pWorld = effect->m_pEffect->GetVariableByName("World")->AsMatrix();
	effect->m_pProjection = effect->m_pEffect->GetVariableByName("Projection")->AsMatrix();

	return effect;
}

void CEffect::Begin()
{
	D3D10_PASS_DESC passDesc;
	HRESULT hr = this->m_pCurrTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	if(this->m_pInputLayout == NULL)
	{
		hr = this->m_pGraphicsDevice->m_pGraphicsDevice->CreateInputLayout(
			this->m_pGraphicsDevice->m_pCurrVertexDeclaration,
			this->m_pGraphicsDevice->m_iVertexDeclarationElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&this->m_pInputLayout);
	}

	this->m_pGraphicsDevice->m_pGraphicsDevice->IASetInputLayout(this->m_pInputLayout);
}

void CEffect::End()
{

}