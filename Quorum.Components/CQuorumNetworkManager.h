#include "Quorum.Components.h"

class QUORUM_COMPONENTS_API CQuorumNetworkManager : public CGameNetworkManager
{
	private:
		#pragma region Static Variables
		static CQuorumSceneManager* g_pSceneManager;
		#pragma endregion

	protected:
		#pragma region Methods
		virtual void ProcessPacket(Packet);
		#pragma endregion

	public:
		#pragma region Constructors and Finalizers
		CQuorumNetworkManager(CGame*, char*, char*, char*);
		#pragma endregion

		#pragma region Overriden Methods
		virtual void Initialize();
		#pragma endregion
};