#ifndef __PUZZLEMANAGER_H__
#define __PUZZLEMANAGER_H__

#include "Module.h"
#include "Puzzle.h"
#include "List.h"

class PuzzleManager : public Module
{
public:

	PuzzleManager();

	// Destructor
	virtual ~PuzzleManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Additional methods
	void TriggerPuzzle(int id);

public:

	Puzzle* currentPuzzle = nullptr;

	List<Puzzle*> puzzles;
	List<Puzzle*> activePuzzles;
	List<Puzzle*> completedPuzzles;

};

#endif // __QUESTMANAGER_H__