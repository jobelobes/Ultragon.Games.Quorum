/**
 * @description 
 *   Interface for the CQuorumMissionManager that allows the return of the mission manager
 *	 and all its components. The service will be used mainly in the initialization of the game
 *	 and the network components
 *
 * @version 1.0.0.0
 * @author	Eric Moreau
 *
 */

class QUORUM_COMPONENTS_API IMissionManager : public IGameService
{

	public:
		#pragma region IGameService Methods
		virtual char* Get_Type();
		#pragma endregion

		#pragma region Methods
		virtual CMission* Get_CurrentMission() = 0;
		virtual void Set_CurrentMission(int index) = 0;
		virtual bool Get_IsGoalReached() = 0;
		#pragma endregion
};