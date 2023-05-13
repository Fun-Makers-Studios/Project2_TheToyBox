#ifndef _MODULECONTROLLER_H_
#define _MODULECONTROLLER_H_

#include "SDL/include/SDL.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "SDL/include/SDL_gamecontroller.h"

#define MAX_BUTTONS 150
#define DEATHZONE  15000
#define MAX_GAME_CONTROLLERS 1

class ModuleController : public Module
{
public:
  
    ModuleController();
    
    // Destructor
    virtual ~ModuleController();

    // Called before render is available
    bool Awake(pugi::xml_node& config);

    // Called before the first frame
    bool Start();

    // Called before all Updates
    bool PreUpdate();

    // Called each loop iteration
    bool Update();

    // Called before all Updates
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

public:

    SDL_Event event;

    //GAME CONTROLLER 1

    SDL_GameController* Controller_player1 = nullptr;

    bool Controller_player1_Connected = false;                //controller connected?

    //values for the Joysticks Axes
    //LEFT Joystick
    int Controller_player1_LAxisX = 0;
    int Controller_player1_LAxisY = 0;

    //RIGHT Joystick
    int Controller_player1_RAxisX = 0;
    int Controller_player1_RAxisY = 0;

    //bools for the Joysticks buttons
    bool controller_player1_A_pressed = false;                //button A pressed?
    bool controller_player1_B_pressed = false;                //button B pressed?
    bool controller_player1_X_pressed = false;                //button X pressed?
    bool controller_player1_Y_pressed = false;                //button Y pressed?

    bool controller_player1_Start_pressed = false;            //button Start pressed?
    bool controller_player1_Back_pressed = false;             //button Back pressed?
    bool controller_player1_Home_pressed = false;             //button Home pressed?

    bool controller_player1_R1_pressed = false;    //button RightShoulder pressed?
    bool controller_player1_L1_pressed = false;     //button LeftShoulder pressed?

    bool controller_player1_L3_pressed = false;               //button L3 pressed?
    bool controller_player1_R3_pressed = false;               //button R3 pressed?

    bool controller_player1_UPArrow_pressed = false;          //button UP Arrow pressed?
    bool controller_player1_DOWNArrow_pressed = false;        //button DOWN Arrow pressed?
    bool controller_player1_LEFTArrow_pressed = false;        //button LEFT Arrow pressed?
    bool controller_player1_RIGHTArrow_pressed = false;       //button RIGHT Arrow pressed?

};
#endif