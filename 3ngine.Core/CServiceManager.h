#include "3ngine.Core.h"

class CORE_API CServiceManager
{
	private:
		CGame* m_pGame;
		std::vector<IGameService*> m_lComponents;

	protected:
		CServiceManager(CGame*);
		~CServiceManager();

		void Initialize();

	public:
		int Get_Count();

		void Clear();
		void AddService(IGameService*);
		void RemoveService(IGameService*);
		IGameService* GetService(char* serviceName);

		friend class CGame;
};