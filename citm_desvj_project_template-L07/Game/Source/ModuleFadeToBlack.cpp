#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

ModuleFadeToBlack::ModuleFadeToBlack() : Module()
{
	name.Create("fade");
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Awake(pugi::xml_node& config)
{
	LOG("Init FadeScreen");
	bool ret = true;
	

	return ret;
}

bool ModuleFadeToBlack::Start()
{
	bool ret = true;
	width = app->configNode.child("window").child("resolution").attribute("width").as_int();
	height = app->configNode.child("window").child("resolution").attribute("height").as_int();
	size = app->configNode.child("window").child("resolution").attribute("scale").as_int();

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		screenRect = { 0, 0, width * size , height * size };

	}

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	return true;
}

bool ModuleFadeToBlack::PreUpdate() {
	return true;
}

bool ModuleFadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) {
		return true;
	} 

	if (currentStep == Fade_Step::TO_BLACK)
	{
		moduleToDisable->Disable();
		moduleToEnable->Enable();

		currentStep = Fade_Step::FROM_BLACK;

		/*++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();
			
			currentStep = Fade_Step::FROM_BLACK;
			
		}*/
	}
	else
	{
		currentStep = Fade_Step::NONE;

		/*--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}*/
	}
	fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	/*SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	LOG("%f", fadeRatio);*/


	return true;
}

bool ModuleFadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	//SDL_RenderFillRect(app->render->renderer, NULL);

	return true;
}


bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if(currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
		
		ret = true;
	}

	return ret;
}