#ifndef TDG_BACKGROUND_H
#define TDG_BACKGROUND_H

#include "TDG_StoredTiles.h"
#include "TDG_Tile.h"
#include "TDG_GameSpecs.h"
#include "TDG_GUI.h"
#include "TDG_SpriteLoader.h"
#include <iostream>

#define TILE_WIDTH 32
#define TILE_HIGHT 32

using namespace std;

class TDG_Background
{
    public:
        TDG_Background();
        virtual ~TDG_Background();

        bool create(TDG_GUI* gui, Room* room);

    protected:

    private:
        int tileRows, tileColumns;

        TDG_StoredTiles* sTiles;

        TDG_Tile*** tileArrangement;
};

#endif // TDG_BACKGROUND_H
