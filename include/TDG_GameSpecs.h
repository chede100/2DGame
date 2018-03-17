#ifndef TDG_GAMESPECS_H
#define TDG_GAMESPECS_H

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
    int playerCharID;
    int roomID;
}SavePoint;

typedef struct
{
    int id;
    int posX;
    int posY;
}Entity;

typedef struct
{
    string roomName;
    Entity player;
    list<Entity> npc;
    list<Entity> obj;

    list<int> tileIDs;
    int tileRows;
    int tileColumns;
    int** tileIDArrangement;
    bool** enviromentCollision;

}Room;

class TDG_GameSpecs
{
    public:
        TDG_GameSpecs();
        virtual ~TDG_GameSpecs();

        bool load();

        Room* getRoom();
        Options* getOpt();
        SavePoint* getSPoint();

    protected:

    private:
        bool loadRoom(int roomID);
        bool loadSPoint();
        bool loadOpt();

        Room* room;
        SavePoint* sPoint;
        Options* opt;

        int nextInt(vector<string>& entries);
        string& nextString(vector<string>& entries);
        vector<string> split(const string& str, char delimiter);
};

#endif // TDG_GAMESPECS_H
