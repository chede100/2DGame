#ifndef TDG_BACKGROUND_H
#define TDG_BACKGROUND_H

#include "TDG_StoredTiles.h"
#include "TDG_Tile.h"
#include "TDG_GameSpecs.h"
#include "TDG_GUI.h"
#include "TDG_SpriteLoader.h"
#include <iostream>
#include <SDL.h>

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
        int tileWidth, tileHight;

        TDG_StoredTiles* sTiles;

        TDG_Tile*** tileArrangement;
};

#endif // TDG_BACKGROUND_H
