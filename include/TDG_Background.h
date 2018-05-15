#ifndef TDG_BACKGROUND_H
#define TDG_BACKGROUND_H

#include "TDG_StoredTiles.h"
#include "TDG_Tile.h"
#include "TDG_GUI.h"
#include "TDG_SpriteLoader.h"

using namespace std;

class TDG_Background
{
    public:
        TDG_Background();
        virtual ~TDG_Background();

        bool create(TDG_GUI* gui, Room* room);
        bool createEmpty(TDG_GUI* gui, int rows, int columns);

        bool renderAtPos(TDG_GUI* gui, int x, int y);

        bool isTileImpassable(int row, int column);
        bool isGate(int row, int column);

        void print(ofstream* out);

        TDG_Tile* getTile(int row, int column);
        int getTileWidth();
        int getTileHight();
        int getTileRows();
        int getTileColumns();
        void getGate(Gate* gate, int row, int column);

    protected:

    private:
        bool initRoom(TDG_GUI* gui, int rows, int columns);
        SDL_Texture* getTileImage(int id);

        int tileRows, tileColumns;
        int tileWidth, tileHight;

        TDG_StoredTiles* sTiles;
        TDG_Tile** tileArrangement;

        list<Gate> gates;
};

#endif // TDG_BACKGROUND_H
