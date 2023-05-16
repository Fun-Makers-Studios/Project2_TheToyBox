#include "PuzzleManager.h"
#include "App.h"
#include "Textures.h"
#include "ButtonOrderPuzzle.h"
#include "MirrorPuzzle.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

PuzzleManager::PuzzleManager() : Module()
{
	name.Create("puzzlemanager");
}

// Destructor
PuzzleManager::~PuzzleManager()
{}

// Called before render is available
bool PuzzleManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Puzzle Manager");
	bool ret = true;
	
	for (pugi::xml_node node = config.child("puzzle"); node; node = node.next_sibling("puzzle"))
	{
		Puzzle* puzzle;
		switch ((PuzzleType)node.attribute("type").as_int())
		{
		case PuzzleType::BUTTON_ORDER:
			puzzle = new ButtonOrderPuzzle(node);
			break;
		case PuzzleType::MIRROR:
			puzzle = new MirrorPuzzle(node);
			break;
		default:
			break;
		}

		puzzles.Add(puzzle);
	}

	return ret;
}

bool PuzzleManager::Start()
{
	LOG("Starting Quest Manager");
	bool ret = true;

	return ret;
}

bool PuzzleManager::Update(float dt)
{
	bool ret = true;

	ListItem<Puzzle*>* item;
	Puzzle* pPuzzle = NULL;

	for (item = activePuzzles.start; item != NULL; item = item->next)
	{
		pPuzzle = item->data;

		if (pPuzzle->Update() == true)
		{
			activePuzzles.Del(item);

			ListItem<Puzzle*>* pitem = puzzles.start;
			while (pitem != nullptr)
			{
				Puzzle* item = pitem->data;
				if (item->orderID == pPuzzle->nextPuzzleId)
				{
					activePuzzles.Add(item);
					break;
				}

				pitem = pitem->next;
			}

			completedPuzzles.Add(pPuzzle);
		}
	}

	LOG("PUZZLES: %d", puzzles.Count());
	LOG("PUZZLES ACTIVE: %d", activePuzzles.Count());

	return ret;
}

void PuzzleManager::TriggerPuzzle(int id)
{
	ListItem<Puzzle*>* pitem = puzzles.start;

	while (pitem != nullptr)
	{
		Puzzle* item = pitem->data;
		if (item->orderID == id) {
			item->LoadAssets(app->configNode);
			activePuzzles.Add(item);
			break;
		}

		pitem = pitem->next;
	}
}


