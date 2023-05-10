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

public:
	//flags
	bool debug;
	bool variables = false;
	bool camLimits = false;
	bool godMode = false;
	bool freeCam = false;
	bool teleport = false;
	bool drawColliders = false;
	bool controlFPS = false;

	// FPS
	microseconds timePerCycle;
	microseconds elapsedFrame;
	int desiredFPSmic;
	int desiredFPS;

private:

	Player* nullPlayer = nullptr;
};

#endif //__DEBUG_H__