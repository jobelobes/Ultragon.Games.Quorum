#include "Quorum.h"

class VolumeOptionsScreen : public CGameScreen//CMenuScreen
{
private:
	bool m_bIsEnabled;
	std::vector<CSprite2D> sprites;
	int m_iSelectedEntry;
	std::vector<char*> m_sMenuEntries;
	/*IScreenManager* m_pScreenManager;*/
	CSpriteBatch* m_pSpriteBatch;
	IInputService* m_pInputService;
	
public:
	VolumeOptionsScreen(CGame*);
	~VolumeOptionsScreen();

	void Initialize();
	void HandleWindowsInput(IWindowsInputService*, CGameTime*);
	void HandleXboxInput(IXBoxInputService*, CGameTime*);
	void Update(CGameTime*);
	void Draw(CGameTime*);
	void Activate(CGameScreen*);
	void Deactivate();

	#pragma region Overridden Methods
	virtual char* Get_Type();
	#pragma endregion
};