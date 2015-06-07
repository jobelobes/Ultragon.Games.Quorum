#include "Quorum.h"

class TitleScreen : public CGameScreen//CMenuScreen
{
private:
	
public:
	TitleScreen(CGame*);
	~TitleScreen();

	virtual void Initialize();
	virtual void LoadContent();
	virtual void HandleWindowsInput(IWindowsInputService*, CGameTime*);
	virtual void Update(CGameTime*);
	virtual void Draw(CGameTime*);
};