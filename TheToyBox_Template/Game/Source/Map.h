#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	// Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	// Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

	List<Property*> list;
};

// Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	// Store custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// Short function to get the gid value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct Object
{
	int id;
	int x, y;
	int* chainPoints;
	int size;

	inline int Get(int x) const
	{
		return chainPoints[x];
	}
	Object() : chainPoints(NULL)
	{}

	~Object()
	{
		RELEASE(chainPoints);
	}
};
struct ObjectGroup
{
	SString	name;
	int id;

	List<Object*> objects;
};

// Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;

	//: Add a list/array of layers to the map
	List<MapLayer*> maplayers;

	List<ObjectGroup*> mapObjectGroups;
	
};


class Map : public Module
{
public:

    Map(bool startEnabled);

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* scene);

	// Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y);

	// L12: Create walkability map for pathfinding
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	bool CreateColliders();

private:

	bool LoadMap(pugi::xml_node mapFile);

	// Create and call a private function to load a tileset
	bool LoadTileSet(pugi::xml_node mapFile);

	// L05
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	bool LoadObject(pugi::xml_node& node, Object* object);
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup* objectGroup);
	bool LoadAllObjectGroups(pugi::xml_node mapNode);

	TileSet* GetTilesetFromTileId(int gid) const;

	// Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	

public: 

	// Declare a variable data of the struct MapData
	MapData mapData;


private:

    SString mapFileName;
	SString mapFolder;
    bool mapLoaded;
};

#endif // __MAP_H__