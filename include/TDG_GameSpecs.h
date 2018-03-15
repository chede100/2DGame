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
    int winWidth = 0;
    int winHight = 0;
    int fpsCap = 0;
    bool fullscreen = false;
}Options;

typedef struct
{
    int playerCharID = 0;
    int roomID = 0;
}SavePoint;

typedef struct
{
    int id = 0;
    int posX = -1;
    int posY = -1;
}Entity;

typedef struct
{
    string roomName;
    Entity player;
    list<Entity> npc;
    list<Entity> obj;

    list<int> tileIDs;
    int tileRows = 0;
    int tileColumns = 0;
    int** tileIDArrangement = NULL;
    bool** enviromentCollision = NULL;

}Room;

class TDG_GameSpecs
{
    public:
        TDG_GameSpecs();
        virtual ~TDG_GameSpecs();

        bool load();

        Room* getRoomSpecs();
        Options* getOptSpecs();
        SavePoint* getSPointSpecs();

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
