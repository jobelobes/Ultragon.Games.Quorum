#include "3ngine.Core.h"

class CGameWindow;
class CGraphicsDevice;
class CGameComponentManager;
class CServiceManager;
class CInput;

class CORE_API CGame
{
	private:
		#pragma region Variables
		CGameTime* m_pTimer;
		CGameComponentManager* m_pComponentManager;
		CServiceManager* m_pServicesManager;
		#pragma endregion

	public:
		CGame();
		~CGame();

		void Run();
		void Exit();

		#pragma region Properties
		CGameComponentManager* Get_Components();
		CServiceManager* Get_Services();
		#pragma endregion

	protected:
		#pragma region Methods
		virtual void Initialize();
		virtual void LoadContent();
		
		virtual void Update(CGameTime*);
		
		virtual void BeginRun();
		virtual void EndRun();

		virtual bool BeginDraw();
		virtual void Draw(CGameTime*);
		virtual void EndDraw();
		#pragma endregion
};