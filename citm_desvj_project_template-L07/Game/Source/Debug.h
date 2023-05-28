#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"
#include "Player.h"
#include <chrono>
using namespace std::chrono;

#define DEBUG_BOX 225

class Debug : public Module
{
public:

	Debug();
	virtual ~Debug();

	bool Start();
	bool Update(float dt);	// Switch debug on/off
	bool PostUpdate();		// Draw if debug true

	void DrawDebug();
	void DrawColliders();
	void DrawEntities();
	void DrawGuiBounds();

	void BlitTextDebug(std::string text, uchar tab);
	void BlitTextDebug(const char* text, uchar tab);

public:

	// Flags
	bool debug;
	int fontID = 3;

	bool freeCam = false;
	bool camLimits = false;
	bool drawGUIbounds = false;
	bool drawColliders = false;
	bool godMode = false;
	bool capFPS = false;

	bool variables = false;
	bool menus = false;
	bool teleport = false;

	// Blit
	int debugX = 0;
	int debugY = 0;

	int lineSpacing = 16;
	int lineNum = 0;

	// FPS
	microseconds timePerCycle;
	microseconds elapsedFrame;
	int desiredFPSmic;
	int desiredFPS;

private:

	Player* nullPlayer = nullptr;
};

#endif //__DEBUG_H__