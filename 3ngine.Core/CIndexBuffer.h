#include "3ngine.Core.h"

class CORE_API CIndexBuffer
{
	private:
		CGraphicsDevice* m_pGraphicsDevice;
		D3D10_USAGE m_eBufferUsage;
		ID3D10Buffer* m_iBuffer;
		int m_iLength;
		int m_iStride;	

		void CreateBuffer(void*);

	public:
		CIndexBuffer(CGraphicsDevice*, int, int, D3D10_USAGE);
		~CIndexBuffer();

		void SetData(void*);

		int Get_SizeInBytes();

		friend class CGraphicsDevice;
};
