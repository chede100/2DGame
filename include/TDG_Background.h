#ifndef TDG_BACKGROUND_H
#define TDG_BACKGROUND_H

#include "TDG_StoredTiles.h"
#include "TDG_Tile.h"
#include "TDG_Window.h"
#include "TDG_SpriteLoader.h"

using namespace std;

class TDG_Background
{
    public:
        TDG_Background();
        virtual ~TDG_Background();

        bool create(TDG_Window* win, Room* room);
        bool createEmpty(TDG_Window* win, int rows, int columns);

        bool renderAtPos(TDG_Window* win, int x, int y);

        bool addImage(TDG_Window* win, TDG_SpriteLoader* sprite, int id);
        bool isTileImgStored(int id);
        void removeUnusedImages();

        bool isTileImpassable(int row, int column);
        bool isGate(int row, int column);

        void print(ofstream* out);

        TDG_Tile* getTile(int row, int column);
        TDG_StoredTiles* getTileImage(int tileID);
        int getTileWidth();
        int getTileHight();
        int getTileRows();
        int getTileColumns();
        void getGate(Gate* gate, int row, int column);

    protected:

    private:
        bool initRoom(TDG_Window* win, int rows, int columns);

        int tileRows, tileColumns;
        int tileWidth, tileHight;

        TDG_StoredTiles* sTiles;
        TDG_Tile** tileArrangement;

        list<Gate> gates;
};

#endif // TDG_BACKGROUND_H
