#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GuiControl;

class Module
{
public:

	Module(bool startEnabled = false) : state(startEnabled)
	{}

	bool Init()
	{
		return true;
	}

	// Called before render is available
	// Sending config file to all modules
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

	void Enable() {
		if (!state) {
			state = true;
			Start();
		}
	}

	void Disable() {
		if (state) {
			state = false;
			CleanUp();
		}
	}

	inline bool IsEnabled() const { return state; }

public:

	SString name;
	bool state;

};

#endif // __MODULE_H__