#ifndef __MENU_PARTY_H__
#define __MENU_PARTY_H__

#include "Menu.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "Item.h"

#define MAX_INVENTORY_SLOTS_ROWS 5
#define MAX_INVENTORY_SLOTS_COLS 6

struct SDL_Texture;

struct InventorySlot
{
	GuiButton* button;
	Item* item = nullptr;
};

class MenuParty : public Menu
{
public:

	MenuParty();
	virtual ~MenuParty();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void SwapItems();

	void BlitPartyStats();

	int BlitTextStats(const char* text, int lineNum);

public:

	// UI
	GuiButton* partyMember1Button = nullptr;
	GuiButton* partyMember2Button = nullptr;
	GuiButton* partyMember3Button = nullptr;
	GuiButton* partyMember4Button = nullptr;

	enum class ControlID : uint32
	{
		PARTY_1,
		PARTY_2,
		PARTY_3,
		PARTY_4,
		I_SLOT
	};

	List<InventorySlot*> inventorySlotList;

private:

	ControlID partyMemberSelected = ControlID::PARTY_1;

	//Textures
	SDL_Texture* partyMenuImg = nullptr;
	const char* partyMenuImgPath = nullptr;
	SDL_Rect rectTexture = { 0, 0, 519, 311 };

	const char* zeroImgPath = nullptr;
	SDL_Texture* zeroImg = nullptr;

	const char* sophieImgPath = nullptr;
	SDL_Texture* sophieImg = nullptr;

	uint slotNumP = 0;
};

#endif // __MENU_PARTY_H__