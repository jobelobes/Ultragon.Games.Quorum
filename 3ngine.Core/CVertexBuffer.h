#include "3ngine.Core.h"

class CORE_API CVertexBuffer
{
	private:
		CGraphicsDevice* m_pGraphicsDevice;
		D3D10_USAGE m_eBufferUsage;
		ID3D10Buffer* m_iBuffer;
		D3D10_INPUT_ELEMENT_DESC* m_aVertexElements;
		int m_iLength;
		int m_iStride;	

		void CreateBuffer(void*);

	public:
		CVertexBuffer(CGraphicsDevice*, int, int, D3D10_USAGE);
		~CVertexBuffer();

		void SetData(void*);

		int Get_SizeInBytes();

		friend class CGraphicsDevice;
};
