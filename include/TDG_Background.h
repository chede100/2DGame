#ifndef TDG_BACKGROUND_H
#define TDG_BACKGROUND_H

#include "TDG_StoredTiles.h"
#include "TDG_Tile.h"
#include "TDG_GameSpecs.h"
#include "TDG_GUI.h"
#include "TDG_SpriteLoader.h"
#include <iostream>
#include <SDL.h>

#define TILE_WIDTH 32
#define TILE_HIGHT 32
#define TILE_SPRITE_MAX_ROWS 50
#define TILE_SPRITE_MAX_COLUMNS 50

using namespace std;

class TDG_Background
{
    public:
        TDG_Background();
        virtual ~TDG_Background();

        bool create(TDG_GUI* gui, Room* room);

        bool renderAtPos(TDG_GUI* gui, int x, int y, int scaleFaktor);

    protected:

    private:
        SDL_Texture* getTileImage(int id);

        int tileRows, tileColumns;

        TDG_StoredTiles* sTiles;

        TDG_Tile*** tileArrangement;
};

#endif // TDG_BACKGROUND_H
