#ifndef TDG_GAMESPECS_H
#define TDG_GAMESPECS_H

#include "TDG_Information.h"
#include <cstddef>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <SDL.h>

using namespace std;

typedef struct
{
    int winWidth;
    int winHight;
    int fpsCap;
    bool fullscreen;
}Options;

typedef struct
{
    int id;
    int posX;
    int posY;
    MovementStatus firstStatus;

    string name;
    int animationID;
    int width, hight;
    double speed;
}Entity;

typedef struct
{
    int row;
    int column;
    int destination;
}Gate;

typedef struct
{
    int roomID;
    Entity player;
}SavePoint;

typedef struct
{
    string roomName;
    list<Entity> npc;
    list<Entity> obj;

    list<Gate> gates;

    list<int> tileIDs;
    int tileRows;
    int tileColumns;
    int** tileIDArrangement;
    int** tileRotationDegree;
    bool** enviromentCollision;
    int** flipTile;

}Room;

class TDG_GameSpecs
{
    public:
        TDG_GameSpecs();
        virtual ~TDG_GameSpecs();

        bool load();
        bool loadRoom(int roomID);

        Room* getRoom();
        Options* getOpt();
        SavePoint* getSPoint();

    protected:

    private:
        bool loadSPoint();
        bool loadOpt();

        Room* room;
        SavePoint* sPoint;
        Options* opt;

        bool loadEntity(EntityTyp typ, Entity* e);

        double nextDouble(vector<string>& entries);
        int nextInt(vector<string>& entries);
        void nextString(vector<string>& entries, string& input);
        vector<string> split(const string& str, char delimiter);

        MovementStatus stringToMoveStatus(string& status);
};

#endif // TDG_GAMESPECS_H
